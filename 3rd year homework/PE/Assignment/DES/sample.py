def even(ran):
    for i in range(ran):
        if i % 2 == 0:
            yield i * 4

gen = even(20)

print(next(gen))
print(next(gen))
print(next(gen))