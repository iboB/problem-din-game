import math
a, b, c = map(int, input().split())
amax=int(math.log2(c/a)+1)
bmax=int(math.log2(c/b)+1)
i=0
j=0
P=[]
while i<=amax:
    j=0
    while j<=bmax:
        if a*((2**i)-1) + b*((2**j)-1) == c:
            P.extend('A'*int(i))
            P.extend('B'*int(j))
            i=amax+1
            break
        j+=1
    i+=1
for i in P:
    print('C->' + i)
