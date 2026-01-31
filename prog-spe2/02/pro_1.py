# 最大値と最小値を出力するプログラム

num = []

for _ in range(10):
    v = int(input('Input an integer: '))
    num.append(v)

print(f'max: {max(num)}')
print(f'min: {min(num)}')
    

# こっちでもいい_1
# is_first = True

# for _ in range(10):
# 	value = int(input('Input a integer: '))
# 	if is_first:
# 		max = value
# 		min = value
# 		is_first = False
# 	if max < value:
# 		max = value
# 	if min > value:
# 		min = value

# print(f'max: {max}')
# print(f'min: {min}')


# こっちでもいい_2
# value = int(input('Input an integer: '))
# max_value = min_value = value

# for _ in range(9):
#     value = int(input('Input an integer: '))
#     if value > max_value:
#         max_value = value
#     if value < min_value:
#         min_value = value

# print(f'max: {max_value}')
# print(f'min: {min_value}')
