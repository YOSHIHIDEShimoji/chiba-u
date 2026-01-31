# get_spotify_data_oauth.py
#
# NOTE (for report):
# This script uses Spotify Web API with OAuth (Authorization Code Flow)
# to collect track metadata and audio features from two playlists
# (Japan-side and Global-side). In the submitted version,
# CLIENT_ID and CLIENT_SECRET are masked as "xxx" for security reasons.
#
# 実運用時は CLIENT_ID, CLIENT_SECRET に自分の値を入れて実行し、
# 生成される "spotify_top50_jp_global.csv" を解析に用いる。

import spotipy
from spotipy.oauth2 import SpotifyOAuth
import pandas as pd
import os

def load_client_keys(path="./_ignore/client.csv"):
    """CLIENT_ID と CLIENT_SECRET を CSV から読み込む"""
    if not os.path.exists(path):
        raise FileNotFoundError(f"client.csv が見つかりません: {path}")

    df = pd.read_csv(path, header=None)
    # 0 列目がキー名、1 列目が値
    key_map = dict(zip(df[0], df[1]))

    return key_map.get("CLIENT_ID"), key_map.get("CLIENT_SECRET")


# ====== 認証情報（提出用では xxx にする） ======
CLIENT_ID, CLIENT_SECRET = load_client_keys()

REDIRECT_URI = "http://127.0.0.1:8000/callback"
# audio-features, playlist 読み取りだけなら特別な scope は不要だが、
# 念のため playlist 読み取り系 scope を付けておく。
SCOPE = "playlist-read-private"
# =================================================

# ====== 対象プレイリストID ======
# search.py で確認した Japan 側
JAPAN_PLAYLIST_ID = "2s46ODpS4wZTb3OW7xnrLK"

# search で見つけた Global 側
GLOBAL_PLAYLIST_ID = "5FN6Ego7eLX6zHuCMovIR2"

PLAYLISTS = {
    "Japan": JAPAN_PLAYLIST_ID,
    "Global": GLOBAL_PLAYLIST_ID,
}
# =================================


def create_spotify_client() -> spotipy.Spotify:
    """SpotifyOAuth で Spotify クライアントを作成する。"""
    auth_manager = SpotifyOAuth(
        client_id=CLIENT_ID,
        client_secret=CLIENT_SECRET,
        redirect_uri=REDIRECT_URI,
        scope=SCOPE,
        open_browser=True,        # 初回実行時にブラウザを開く
        cache_path=".cache-spotify"  # トークンをキャッシュ
    )
    sp = spotipy.Spotify(auth_manager=auth_manager)
    return sp


def collect_tracks_for_playlist(sp: spotipy.Spotify, playlist_id: str, region_label: str):
    """
    1つのプレイリストから:
      - track name
      - artist name (1人目)
      - popularity
      - audio features (danceability, energy, valence, tempo, acousticness)
    を取得して dict のリストで返す。
    """
    playlist = sp.playlist(playlist_id)

    playlist_name = playlist.get("name")
    items = playlist["tracks"]["items"]

    track_ids = []
    track_meta = []

    for item in items:
        track = item.get("track")
        if track is None:
            continue
        if track.get("id") is None:
            # ローカルファイルなど id が無いものはスキップ
            continue

        track_ids.append(track["id"])
        track_meta.append({
            "track_id": track["id"],
            "track_name": track.get("name"),
            "artist": (track.get("artists") or [{}])[0].get("name"),
            "popularity": track.get("popularity", None),
        })

    # audio_features は最大100件まで一括取得可能
    # 403 対策として、念のため 50件ずつのバッチに分けて投げる
    features_map = {}
    BATCH = 50
    for i in range(0, len(track_ids), BATCH):
        batch_ids = track_ids[i:i+BATCH]
        features_list = sp.audio_features(batch_ids)
        for tid, feat in zip(batch_ids, features_list):
            if feat is None:
                continue
            features_map[tid] = feat

    rows = []
    for meta in track_meta:
        tid = meta["track_id"]
        feat = features_map.get(tid)
        if feat is None:
            continue

        row = {
            "region": region_label,
            "playlist_name": playlist_name,
            "track_id": tid,
            "track_name": meta["track_name"],
            "artist": meta["artist"],
            "popularity": meta["popularity"],
            "danceability": feat.get("danceability"),
            "energy": feat.get("energy"),
            "valence": feat.get("valence"),
            "tempo": feat.get("tempo"),
            "acousticness": feat.get("acousticness"),
        }
        rows.append(row)

    return rows


def main():
    sp = create_spotify_client()

    all_rows = []

    for region_label, pid in PLAYLISTS.items():
        print(f"Collecting from region={region_label}, playlist_id={pid} ...")
        rows = collect_tracks_for_playlist(sp, pid, region_label)
        print(f"  -> collected {len(rows)} tracks")
        all_rows.extend(rows)

    df = pd.DataFrame(all_rows)

    out_path = "spotify_top50_jp_global.csv"
    df.to_csv(out_path, index=False)
    print(f"Saved CSV to: {out_path}")


if __name__ == "__main__":
    main()
