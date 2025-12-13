import spotipy
from spotipy.oauth2 import SpotifyClientCredentials
import pandas as pd
from tqdm import tqdm
import os

def load_client_keys(path="./_ignore/client.csv"):
    """CLIENT_ID と CLIENT_SECRET を CSV から読み込む"""
    if not os.path.exists(path):
        raise FileNotFoundError(f"client.csv が見つかりません: {path}")

    df = pd.read_csv(path, header=None)
    # 0 列目がキー名、1 列目が値
    key_map = dict(zip(df[0], df[1]))

    return key_map.get("CLIENT_ID"), key_map.get("CLIENT_SECRET")

# Spotify API 認証
CLIENT_ID, CLIENT_SECRET = load_client_keys()

auth_manager = SpotifyClientCredentials(
    client_id=CLIENT_ID,
    client_secret=CLIENT_SECRET
)

sp = spotipy.Spotify(auth_manager=auth_manager)

# playlist tracks を取得する関数
def get_playlist_tracks(playlist_ids: dict, playlist_len: int = 50, market: str = "JP"):

    rows = []

    for country in tqdm(playlist_ids.keys()):
        playlist_id = playlist_ids[country]

        playlist = sp.playlist(playlist_id=playlist_id, market=market)
        print("country:", country, "| playlist:", playlist.get("name"))

        tracks = playlist["tracks"]["items"]

        for j, item in enumerate(tracks, start=1):
            track_info = item.get("track")
            if track_info is None:
                continue

            # ---- Track 基本情報 ----
            track_name = track_info.get("name")
            popularity = track_info.get("popularity")
            duration_ms = track_info.get("duration_ms")

            # ---- Album 情報 ----
            album = track_info.get("album") or {}
            release_date = album.get("release_date", "0000")
            release_year = int(str(release_date)[:4])

            # ---- Artist 情報（API呼び出し）----
            artist = (track_info.get("artists") or [{}])[0]
            artist_id = artist.get("id")
            artist_name = artist.get("name")

            artist_popularity = None
            artist_followers = None

            if artist_id:
                try:
                    artist_info = sp.artist(artist_id)
                    artist_popularity = artist_info.get("popularity")
                    artist_followers = artist_info.get("followers", {}).get("total")
                except Exception:
                    pass  # APIエラー時は空欄のまま

            # ---- 行として追加 ----
            rows.append({
                "country": country,
                "weekly_rank": j,
                "track_name": track_name,
                "artist_name": artist_name,

                # 解析で使う特徴量
                "popularity": popularity,
                "duration_ms": duration_ms,
                "release_year": release_year,
                "artist_popularity": artist_popularity,
                "artist_followers": artist_followers,
            })

    return pd.DataFrame(rows)


# ===========================
# プレイリストID（Japan / Global）
# ===========================
playlist_ids = {
    "Japan": "2s46ODpS4wZTb3OW7xnrLK",
    "Global": "5FN6Ego7eLX6zHuCMovIR2",
}


# ===========================
# 実行
# ===========================
if __name__ == "__main__":
    df = get_playlist_tracks(
        playlist_ids=playlist_ids,
        playlist_len=50,
        market="JP",
    )

    print(df.head())
    df.to_csv("spotify_meta.csv", index=False, encoding="utf-8-sig")
    print("Saved: spotify_meta.csv")
