import time

while True:
    n = int(input("Input the number of numbers to process: "))
    if n > 1 and n < 31:
        break

num = []
while(True):
    print("Input the numbers to be processed:")
    string = input()
    numbers = string.split(' ')
    check = True
    for number in numbers:
        try:
            if int(number)<=0 or int(number)>100000:
                check = False
        except: check = False
    if check:
        break
for number in numbers:
    num.append(int(number))

start = time.time()
prime = [True] * 100001
prime[1] = False

for i in range(2, 100001):
    for j in range(2, 100001//i+1):
        if prime[i]:
            if i*j < 100001: prime[i*j] = False

num.sort()
for i in range(n - 1):
    cnt = 0
    for j in range(num[i], num[i + 1] + 1):
        if prime[j]:
            cnt += 1
    print(f"Number of prime numbers between {num[i]} {num[i+1]}: {cnt}")

end = time.time()
print(f"Total execution time using Python is {end - start:.6f} seconds!")
