import pandas as pd
import math
import multiprocessing
import signal
import sys
import time
import os
from ortools.constraint_solver import routing_enums_pb2
from ortools.constraint_solver import pywrapcp

# === 設定 ===
# ファイル名は変更なしでOK（自動で絶対パスに変換します）
FILE_NAME = 'cities2025_30-100/cities100.csv'
SCALE_FACTOR = 1000000
CHUNK_TIME = 60

def get_absolute_path(filename):
    """実行中のファイルと同じ場所にあるcsvの絶対パスを取得"""
    base_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(base_dir, filename)

def create_data_model(file_path):
    """データ読み込み（エラー時は詳細を表示）"""
    try:
        if not os.path.exists(file_path):
            print(f"[エラー] ファイルが見つかりません: {file_path}")
            return None, 0
            
        df = pd.read_csv(file_path, header=None, names=['x', 'y'])
        locations = df[['x', 'y']].values.tolist()
        dist_matrix = {}
        for from_node in range(len(locations)):
            dist_matrix[from_node] = {}
            for to_node in range(len(locations)):
                if from_node == to_node:
                    dist_matrix[from_node][to_node] = 0
                else:
                    x1, y1 = locations[from_node]
                    x2, y2 = locations[to_node]
                    dist = math.sqrt((x1 - x2)**2 + (y1 - y2)**2)
                    dist_matrix[from_node][to_node] = int(dist * SCALE_FACTOR)
        return dist_matrix, len(locations)
    except Exception as e:
        print(f"[データ作成エラー] {e}")
        return None, 0

def solve_tsp_loop(process_id, best_data, file_path):
    """エラーを隠蔽せずに表示するモード"""
    try:
        # print(f"[Core {process_id}] 起動しました...") # デバッグ用
        dist_matrix, num_nodes = create_data_model(file_path)
        
        if not dist_matrix:
            print(f"[Core {process_id}] データ読み込み失敗のため終了します。")
            return

        while True:
            manager = pywrapcp.RoutingIndexManager(num_nodes, 1, 0)
            routing = pywrapcp.RoutingModel(manager)
            
            def distance_callback(from_index, to_index):
                return dist_matrix[manager.IndexToNode(from_index)][manager.IndexToNode(to_index)]
            
            transit_idx = routing.RegisterTransitCallback(distance_callback)
            routing.SetArcCostEvaluatorOfAllVehicles(transit_idx)

            search_params = pywrapcp.DefaultRoutingSearchParameters()
            search_params.local_search_metaheuristic = (
                routing_enums_pb2.LocalSearchMetaheuristic.GUIDED_LOCAL_SEARCH)
            search_params.time_limit.seconds = CHUNK_TIME
            
            strategies = [
                routing_enums_pb2.FirstSolutionStrategy.PATH_CHEAPEST_ARC,
                routing_enums_pb2.FirstSolutionStrategy.SAVINGS,
                routing_enums_pb2.FirstSolutionStrategy.CHRISTOFIDES,
                routing_enums_pb2.FirstSolutionStrategy.LOCAL_CHEAPEST_INSERTION,
                routing_enums_pb2.FirstSolutionStrategy.GLOBAL_CHEAPEST_ARC
            ]
            search_params.first_solution_strategy = strategies[process_id % len(strategies)]

            # 計算実行
            solution = routing.SolveWithParameters(search_params)

            if solution:
                cost = solution.ObjectiveValue() / SCALE_FACTOR
                
                try:
                    current_best = best_data.get('cost', float('inf'))
                    if cost < current_best:
                        # ルート復元
                        index = routing.Start(0)
                        route = []
                        while not routing.IsEnd(index):
                            route.append(str(manager.IndexToNode(index)))
                            index = solution.Value(routing.NextVar(index))
                        route.append(str(manager.IndexToNode(index)))
                        route_str = "-".join(route)

                        # 更新
                        best_data['cost'] = cost
                        best_data['route'] = route_str
                        
                        timestamp = time.strftime('%H:%M:%S')
                        print(f"[{timestamp}] Process {process_id:02d} ★ベスト更新! Cost: {cost:.6f}")
                        
                        # ファイル保存
                        with open("tsp_realtime_best.txt", "w") as f:
                            f.write(f"Cost: {cost}\nRoute: {route_str}")
                except Exception as e:
                    # マネージャー接続エラー等の場合
                    # print(f"通信エラー（終了処理中）: {e}")
                    break

    except Exception as e:
        print(f"[Core {process_id} 重大エラー] {e}")
        import traceback
        traceback.print_exc()
        return

def main():
    multiprocessing.set_start_method('spawn', force=True)

    # パスの確認
    abs_file_path = get_absolute_path(FILE_NAME)
    print(f"読み込み対象ファイル: {abs_file_path}")
    if not os.path.exists(abs_file_path):
        print("【重要】CSVファイルが見つかりません！パスを確認してください。")
        return

    with multiprocessing.Manager() as manager_mem:
        best_data = manager_mem.dict({'cost': float('inf'), 'route': ""})
        processes = []

        try:
            num_cores = multiprocessing.cpu_count()
            print(f"M4チップ {num_cores}コアで計算開始（60秒ごとに更新）。")
            print("停止するには Ctrl + C を押してください（データは保持されます）。")
            print("-" * 50)

            for i in range(num_cores):
                # file_pathを引数で渡すように変更（子プロセスでパス迷子にならないように）
                p = multiprocessing.Process(target=solve_tsp_loop, args=(i, best_data, abs_file_path))
                p.start()
                processes.append(p)

            for p in processes:
                p.join()

        except KeyboardInterrupt:
            print("\n" + "="*50)
            print("中断信号を受信。プロセスを停止しています...")
            for p in processes:
                if p.is_alive():
                    p.terminate()
            
            final_cost = best_data.get('cost')
            if final_cost != float('inf'):
                print(f"最終ベストコスト: {final_cost:.6f}")
                print("結果は 'tsp_realtime_best.txt' に保存されています。")
            else:
                print("有効な解が見つかる前に終了しました。")
            print("="*50)
            sys.exit(0)

if __name__ == '__main__':
    main()