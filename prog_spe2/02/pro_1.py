# 最大値と最小値を出力するプログラム

num = []

for _ in range(10):
    v = int(input('Input an integer: '))
    num.append(v)

print(f'max: {max(num)}')
print(f'min: {min(num)}')
    

# こっちでもいい

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

