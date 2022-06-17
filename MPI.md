## 特殊数据类型

### VECTOR数据类型

```c
int MPI_Type_vector(
	int count,				// 小块个数
	int blocklength,		// 每小块的数据长度
	int stride,				// 每小块之间距离
	MPI_Datatype oldtype,	// 原始数据类型
	MPI_Datatype *newtype	// 返回数据结构
)
```

### Index数据类型

```c
int MPI_Type_indexed(
	int count,					// 小块个数
	int *array_of_blocklengths,	// 每小块的数据数据长度
	int *array_of_displacements,// 每小块与首地址的偏移量
	MPI_Datatype oldtype,		// 原始数据类型
	MPI_Datatype *newtype		// 返回数据结构
)
```

### struct数据类型

```c
int MPI_Type_struct(
	int count,						// 小块个数
	int *array_of_blocklengths,		// 每小块的数据数据长度
	int *array_of_displacements,	// 每小块与首地址的偏移量
	MPI_Datatype *array_of_types,	// 原始数据类型
	MPI_Datatype *new_type			// 返回数据结构
)
```

### MPI的数据打包与拆包

```c
int MPI_Pack(
	void *inbuf,			
	int incount,
	MPI_Datatype datatype,
	void *outbuf,
	int outsize,
	int *position,
    MPI_Comm comm
)

int MPI_Unpack(
	void *inbuf,			
	int insize,
	int *position
	void *outbuf,
	int outcount,
	MPI_Datatype datatype,
    MPI_Comm comm
)

int MPI_Pack_size(
	int incount, 
	MPI_Datatype datatype,
    MPI_Comm comm,
    int *size
)
```

### 数据类型辅助函数

```c
int MPI_Type_commit(MPI_Datatype *datatype)
// 任何自定义后的数据类型
// 需要对其进行commit函数，方可使用
```



## MPI聚合通信

### 障碍同步 MPI_ Barrier

```c
// 所有进程执行这个函数后，立即返回，不让就等待
int MPI_Barrier(MPI_Comm comm);
```

### 广播 MPI_Bcast

```c
int MPI_Bcast(
	void *buffer,
	int count,
	MPI_Datatype datatype,
	int root,
	MPI_Comm comm
)
// 将root进程种的buffer数据在所有进程间同步
```

### 收集 MPI_Gather

```c
int MPI_Gather(
	void *sendbuf,
	int sendcount, // 对每个进程发送的数据数量
	MPI_Datatype sendtype,
	void *recvbuf,
	int recvcount, // 从每个进程接受的数据数量
	MPI_Datatype recvtype,
	int root,
	MPI_Comm comm
)

// 对全部进程进行gather操作
int MPI_Allgather(
	void *sendbuf,
	int sendcount, // 对每个进程发送的数据数量
	MPI_Datatype sendtype,
	void *recvbuf,
	int recvcount, // 从每个进程接受的数据数量
	MPI_Datatype recvtype,
	MPI_Comm comm
)

```



### 散播 MPI_Scatter

```c
int MPI_Scatter(
	void *sendbuf,
	int sendcount,			// 向每个进程散发的数据个数
	MPI_Datatype sendtype,
	void *recvbuf,
	int recvcount,			// 其他进程从root进程接受的数据个数
	MPI_Datatype recvtype,
	int root,
	MPI_Comm comm
)
```



### 全交换 MPI_Alltoall

```c
int MPI_Alltoall(
	void *sendbuf,
	int sendcount,
	MPI_Datatype sendtype,
	void *recvbuf,
	int recvcount,
	MPI_Datatype recvtype,
	MPI_Comm comm
)
int MPI_Alltoallv(
	void *sendbuf,
	int *sendcounts,
	int *sdispls,
	MPI_Datatype sendtype,
	void *recvbuf,
	int *recvcounts,
	int *rdispls,
	MPI_Datatype recvtype,
	MPI_Comm comm
)
```

### 归约 MPI_Reduce

```c
int MPI_Reduce(
	void *sendbuf,
	void *recvbuf,
	int count,		// 数据长度
	MPI_Datatype datatype,
	MPI_Op op,		// 操作(MPI_SUM,MPI_SUB...)
    int root,		// 数据最终汇总的目标进程，通常为root进程(iam == 0)
	MPI_Comm comm
)
```

