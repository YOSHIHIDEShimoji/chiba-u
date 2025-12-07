import spotipy
from spotipy.oauth2 import SpotifyClientCredentials
from spotipy.exceptions import SpotifyException

CLIENT_ID = "xxx"
CLIENT_SECRET = "xxx"

auth = SpotifyClientCredentials(client_id=CLIENT_ID, client_secret=CLIENT_SECRET)
sp = spotipy.Spotify(auth_manager=auth)

SEARCH_TERMS = [
    "Top 50 - Global",
    "Global Top 50",
    "Top 50 Global",
    "Global Hits",
    "World Top 50",
    "Worldwide Top 50",
    "International Top 50",
    "Top Global",
    "Top Hits Global",
    "Global Chart",
    "Top 50",
]

def search_and_check(term: str):
    print(f"\n=== Search: {term} ===")
    results = sp.search(q=term, type="playlist", limit=50)

    playlists = results.get("playlists", {}).get("items", [])
    if not playlists:
        print("No playlists found.")
        return

    for i, pl in enumerate(playlists):
        if not pl:
            continue

        pid = pl.get("id")
        name = pl.get("name")
        owner = (pl.get("owner") or {}).get("display_name")

        # playlist() が通るか確認（404を除外するため）
        try:
            detail = sp.playlist(pid)
            total_tracks = detail["tracks"]["total"]
            print(f"{i}: {name} | id={pid} | tracks={total_tracks} | owner={owner}")
        except SpotifyException as e:
            print(f"{i}: {name} | id={pid} | ERROR {e.http_status}")

def main():
    # 日本版（動作確認）
    search_and_check("Top 50 - Japan")

    # グローバル探索
    for term in SEARCH_TERMS:
        search_and_check(term)

if __name__ == "__main__":
    main()
