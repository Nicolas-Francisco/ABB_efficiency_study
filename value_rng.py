import random

n = 1000000
max_num = 4294967296
min_num = 0

def rng():

    for num in range(100):
        values = random.sample(range(min_num,max_num-1), n)
        f = open(f"Values/values_{num}.txt", "w")
        for i in range(len(values)):
            if i==len(values)-1:
                f.write(str(values[i]))
            else:
                f.write(str(values[i])+"\n")
        f.close()
        print(">archivo numero "+str(num)+ " generado")
        print(">distinto tamaño:",end=" ")
        print(n != len(set(values)))
    
rng()

