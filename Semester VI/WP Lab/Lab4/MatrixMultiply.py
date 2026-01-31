def multiplyMatrix(A,B):
    result=[[0]*len(B[0]) for _ in range(len(A))]
    for i in range(len(A)):
        for j in range(len(B[0])):
            for k in range(len(B)):
                result[i][j]+=A[i][k]*B[k][j]
    return result

r1,c1 = map(int,input("Enter rows and cols of first matrix: ").split())
A = [list(map(int,input().split())) for _ in range(r1)]
r2,c2 = map(int,input("Enter rows and cols of second matrix: ").split())
B = [list(map(int,input().split())) for _ in range(r2)]
print(multiplyMatrix(A,B))
