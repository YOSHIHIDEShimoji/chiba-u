import numpy as np

def bubble_sort(l):
    for i in range(len(l) - 1):
        for j in range(len(l) - 1):
           if l[j] > l[j + 1]:
               l[j], l[j + 1] = l[j + 1], l[j]
    return l

def read_file(path):
    return np.loadtxt(f'{path}', delimiter=',')
    
def cal(data):
    result = []
    for n in range(data.shape[1]):
        col = data[:, n]

        # 平均
        total = 0
        for _ in col:
            total += _
        col_ave = total / len(col)
        # print(f'ave of col_{n}: ', col_ave)

        # 最大、最小
        sorted_l = bubble_sort(col.tolist())
        col_max = sorted_l[-1]
        col_min = sorted_l[0]
        # print(f'max of col_{n}: ',col_max)
        # print(f'min of col_{n}: ',col_min)

        # 中央値
        mid = len(sorted_l) // 2
        if len(sorted_l) % 2 == 0:
            median = (sorted_l[mid - 1] + sorted_l[mid]) / 2
        else:
            median = sorted_l[mid]
        
        # 結果を返す
        result.append([col_ave, col_max, col_min, median]) 
    return np.array(result)
        
def write_file(data):
    np.savetxt('result.csv', 
               data, 
               fmt='%.2f', 
               delimiter=',', 
               header='ave, max, min, median',
               comments='')
    print('./result.csv has been created.')

def main():
    # ファイル読み込み
    csv_file = read_file('./Meteorologic.csv')

    # 統計情報算出
    result = cal(csv_file)

    # ファイル書き込み
    write_file(result)



if __name__ == '__main__':
    main()