import pandas as pd
import math
import multiprocessing
import signal
import sys
from ortools.constraint_solver import routing_enums_pb2
from ortools.constraint_solver import pywrapcp

# === 設定 ===
FILE_PATH = 'cities100.csv'
SCALE_FACTOR = 1000000
CALCULATION_TIME = 86400  # 最大24時間

# 共有変数（メインプロセスで最高値を保持）
best_cost_global = multiprocessing.Value('d', float('inf'))
best_route_global = multiprocessing.Array('c', 4000) # ルート文字列用

def create_data_model():
    df = pd.read_csv(FILE_PATH, header=None, names=['x', 'y'])
    locations = df[['x', 'y']].values.tolist()
    distance_matrix = {}
    for from_node in range(len(locations)):
        distance_matrix[from_node] = {}
        for to_node in range(len(locations)):
            if from_node == to_node:
                distance_matrix[from_node][to_node] = 0
            else:
                x1, y1 = locations[from_node]
                x2, y2 = locations[to_node]
                dist = math.sqrt((x1 - x2)**2 + (y1 - y2)**2)
                distance_matrix[from_node][to_node] = int(dist * SCALE_FACTOR)
    return distance_matrix

def solve_tsp(process_id):
    """各プロセスで実行"""
    try:
        dist_matrix = create_data_model()
        manager = pywrapcp.RoutingIndexManager(len(dist_matrix), 1, 0)
        routing = pywrapcp.RoutingModel(manager)

        def distance_callback(from_index, to_index):
            return dist_matrix[manager.IndexToNode(from_index)][manager.IndexToNode(to_index)]

        transit_idx = routing.RegisterTransitCallback(distance_callback)
        routing.SetArcCostEvaluatorOfAllVehicles(transit_idx)

        search_params = pywrapcp.DefaultRoutingSearchParameters()
        search_params.local_search_metaheuristic = (
            routing_enums_pb2.LocalSearchMetaheuristic.GUIDED_LOCAL_SEARCH)
        search_params.time_limit.seconds = CALCULATION_TIME
        
        # 多様な初期戦略
        strategies = [
            routing_enums_pb2.FirstSolutionStrategy.PATH_CHEAPEST_ARC,
            routing_enums_pb2.FirstSolutionStrategy.SAVINGS,
            routing_enums_pb2.FirstSolutionStrategy.LOCAL_CHEAPEST_INSERTION
        ]
        search_params.first_solution_strategy = strategies[process_id % len(strategies)]

        # 計算開始
        solution = routing.SolveWithParameters(search_params)

        if solution:
            cost = solution.ObjectiveValue() / SCALE_FACTOR
            index = routing.Start(0)
            route = []
            while not routing.IsEnd(index):
                route.append(str(manager.IndexToNode(index)))
                index = solution.Value(routing.NextVar(index))
            route.append(str(manager.IndexToNode(index)))
            return cost, "-".join(route)
    except Exception:
        return float('inf'), ""

def result_callback(result):
    """個別の計算が終わるたびに呼ばれる。ベスト更新なら表示＆保存"""
    global best_cost_global, best_route_global
    cost, route = result
    if cost < best_cost_global.value:
        best_cost_global.value = cost
        print(f"\n[★ ベスト更新] Cost: {cost:.6f}")
        with open("tsp_best_ever.txt", "w") as f:
            f.write(f"Cost: {cost:.6f}\nRoute: {route}")

def signal_handler(sig, frame):
    """Ctrl+C が押された時の処理"""
    print("\n\nユーザーによる中断を検知しました。")
    print("="*40)
    print(f"これまでの最高コスト: {best_cost_global.value:.6f}")
    print("詳細は 'tsp_best_ever.txt' を確認してください。")
    print("="*40)
    sys.exit(0)

def main():
    global best_cost_global
    # 中断シグナルの登録
    signal.signal(signal.SIGINT, signal_handler)

    num_cores = multiprocessing.cpu_count()
    print(f"M4 {num_cores}コアで計算中... 中断するには Ctrl+C を押してください。")
    
    pool = multiprocessing.Pool(processes=num_cores)
    
    for i in range(num_cores):
        pool.apply_async(solve_tsp, args=(i,), callback=result_callback)

    pool.close()
    
    # メインスレッドは終了を待つ（Ctrl+Cが効くようにループさせる）
    try:
        while True:
            time_wait = 1
            if pool._state == 'RUN': # 実行中なら待機
                pool.join()
                break
    except (KeyboardInterrupt, SystemExit):
        pool.terminate()
        sys.exit(0)

if __name__ == '__main__':
    import time
    main()