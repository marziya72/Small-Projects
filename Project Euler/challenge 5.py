smallest = 0
i = 2520
while True:
    for j in range(1,21):
        if i % j != 0:
            break
    else:
        print(i)
        break
    i += 20
print(smallest)