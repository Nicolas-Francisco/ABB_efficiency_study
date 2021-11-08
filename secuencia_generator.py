import random

n = 1000000
max_num = 2**32
min_num = 0

def rng():

    for num in range(1):
        #f = open(f"values_{num}.txt", "w")
        values = random.sample(range(min_num, max_num ), 1000000)
        """ while len(values) != 1000000: 
            value = random.randint(0, 2**32)
            if(value not in values):
                values.append(value)
                f.write(str(value)+",")
            print(len(values)) 
        f.close() """
    return values

nums = rng()  
print(nums)
print( len(nums) != len(set(nums)))

        
        


