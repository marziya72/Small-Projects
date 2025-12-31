sum1 = 0
sum2 = 0
answer = 0

for i in range(1,101):
    sum1 += i*i
    sum2 += i
answer = (sum2*sum2) - sum1
print(answer)