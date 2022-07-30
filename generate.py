import csv
import random

def write():
    lst = [[4,3,4,4,2,5,4,3,5,4,3,5,3,5,4,3,2],
        [1,15,13,11,13,13,15,14,15,11,12,12,11,12,14,11,11],
        [40,15,38,40,2,65,36,16,63,40,18,66,19,63,37,19,4]]
    file = open("a.csv","w")
    for i in lst:
        for j in i:
            file.write(f"{j};")
        file.write("\n")
    file.close()
def read():
    file = open("a.csv")
    lst = []


    with open('table3.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')

        for row in csv_reader:
            lst.append(row)

    for i in lst:
        for j in range(0,len(i)):
            i[j] = int(i[j])
    for j in lst:
        print(j)

def generate():
    rows = 3
    columns = 90000
    delta = 5
    file = open("table2.csv","w")

    lst = [[],[],[]]
    for j in range(0,columns):
        x1 = random.randint(2,30)*random.random()
        x2 = random.randint(2,15)*random.random()
        y= (3.33*x1 + 6.66*x2 + 9.99*x1*x1) #+ randint(-delta,delta)
        delta = random.random()*float(random.randint(-1,1))*0.05*y#
        y += delta
        lst[0].append(x1)
        lst[1].append(x2)
        lst[2].append(y)   
    
    for i in lst:
        for j in range(0,len(i)):
            file.write(f"{i[j]}")
            if (j < len(i)-1):
                file.write(",")
        file.write("\n")
        
    file.close()
    return lst
generate()
