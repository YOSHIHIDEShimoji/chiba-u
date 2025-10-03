def fibonacci(n):
    if n == 0:
        return 0
    if n == 1:
        return 1
    else:
        return fibonacci(n-1) + fibonacci(n-2)

n = int(input('算出数を自然数で入力してください： '))

for i in range(n):
    v = fibonacci(i)
    print(v, end=', ')