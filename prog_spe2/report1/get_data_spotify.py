import spotipy
from spotipy.oauth2 import SpotifyClientCredentials
import pandas as pd
from datetime import date

# Spotify API 認証
CLIENT_ID = "xxx"
CLIENT_SECRET = "xxx"

sp = spotipy.Spotify(
    auth_manager=SpotifyClientCredentials(
        client_id=CLIENT_ID,
        client_secret=CLIENT_SECRET
    )
)

# Japan / Global のプレイリストID
playlist_ids = {
    "Japan": "2s46ODpS4wZTb3OW7xnrLK",
    "Global": "5FN6Ego7eLX6zHuCMovIR2",
}

# プレイリストから楽曲情報を取得
def get_playlist_tracks(playlist_ids, market="JP"):
    rows = []

    for country in playlist_ids:
        # プレイリスト取得
        playlist = sp.playlist(playlist_ids[country], market=market)
        items = playlist["tracks"]["items"]

        for rank, item in enumerate(items, start=1):
            track = item["track"]

            # 楽曲情報
            track_name = track["name"]
            popularity = track["popularity"]
            duration_ms = track["duration_ms"]

            # リリース年
            release_date = track["album"]["release_date"]
            release_year = int(release_date[:4])

            # アーティスト情報
            artist = track["artists"][0]
            artist_name = artist["name"]
            artist_id = artist["id"]

            artist_info = sp.artist(artist_id)
            artist_popularity = artist_info["popularity"]
            artist_followers = artist_info["followers"]["total"]

            # 1行分のデータを追加
            rows.append({
                "country": country,
                "weekly_rank": rank,
                "track_name": track_name,
                "artist_name": artist_name,
                "popularity": popularity,
                "duration_ms": duration_ms,
                "release_year": release_year,
                "artist_popularity": artist_popularity,
                "artist_followers": artist_followers,
            })

    return pd.DataFrame(rows)

# 実行
if __name__ == "__main__":
    df = get_playlist_tracks(playlist_ids, market="JP")

    today = date.today().isoformat()
    filename = f"spotify_top50_japan_global_{today}.csv"

    df.to_csv(filename, index=False, encoding="utf-8-sig")
    print(f"Saved: {filename}")
