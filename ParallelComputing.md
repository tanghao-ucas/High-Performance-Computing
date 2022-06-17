## 矩阵乘法并行计算

$$
C = A * B \\
A \in R^{m\times l}, B \in R^{l \times n}
$$

基于$A$、$B$的不同划分，矩阵乘积的并行算法可分为

+ 行列划分
+ 行行划分
+ 列列划分
+ 列行划分



### 行列划分

$A$：按行划分、$B$：按列划分
$$
AB 
=
\begin{bmatrix}A_0\\A_1\\ \vdots \\ A_{p-1} \end{bmatrix}  
\begin{bmatrix}B_0\ B_1\ \cdots B_{p-1} \end{bmatrix}
= 
\begin{bmatrix}
{A_{0}B_{0}}&{A_{0}B_{1}}&{\cdots}&{A_{0}B_{p-1}}\\
{A_{1}B_{0}}&{A_{1}B_{1}}&{\cdots}&{A_{1}B_{p-1}}\\
{\vdots}&{\vdots}&{\ddots}&{\vdots}\\
{A_{p-1}B_{0}}&{A_{p-1}B_{1}}&{\cdots}&{A_{p-1}B_{p-1}}\\
\end{bmatrix}
$$

```python
# 将每一行分给一个处理器， 共有p行，因此有p个处理器
for i = 0 to (p - 1): 				# 每行有p个数据，因此循环p次
    j = (i + myid) mod p			# 初始位置为对角线上元素， 依次往后循环
    Cj = A * B						# 计算每列的A * B
    src = (my_id + 1) mod p			# 数据发送源
    dest = (myid - 1 + p) mod p		# 数据发送目标， 前一行的处理器
    if(i != p-1)					# 执行发送
   		send(B, dest)
        recv(B, src)
```

$C_j = C_{myid,j}, A = A_{myid}$

### 行行划分

$A$：按行划分、$B$：按行划分
$$
A = \begin{bmatrix}{A_0}^T\ {A_1}^T\ \cdots {A_{p-1}}^T \end{bmatrix} ^T\\
B = \begin{bmatrix}{B_0}^T\ {B_1}^T\ \cdots {B_{p-1}}^T \end{bmatrix} ^T
$$
再将$A$的子块$A_i$按列分块，与B的行分块对应
$$
A_i = 
\begin{bmatrix}A_{i,0}\ A_{i,1}\ \cdots A_{i,p-1} \end{bmatrix}
$$
设$C_i$为与$A_i$相对应的行分块（$A$的行为$C$的行，$B$的列为$C$的列）
$$
C_i = A_i * B = \sum^{p-1}_{j=0}A_{ij}B_j
$$

```python
for i = 0 to (p-1):
	j = (i + myid) mod p
    C = Aj * B
    src = (my_id + 1) mod p
    dest = (myid - 1 + p) mod p
    if(i != p-1)					# 执行发送
   		send(B, dest)
        recv(B, src)
```

$C = C_{myid}, A  = A_{myid, j}$



### 列列划分/列行划分

$A$：按列划分、$B$：按行划分
$$
A = 
\begin{bmatrix}A_0\ A_1\ \cdots A_{p-1} \end{bmatrix} \\ 
B = 
\begin{bmatrix}B_0\ B_1\ \cdots B_{p-1} \end{bmatrix}
$$

$$
C = \sum^{p-1}_{i=0}A_{i}B_{i}\\
C_j = A * B_j = \sum^{p-1}_{j=0}A_{i}B_{ij}
$$


### Cannon算法

