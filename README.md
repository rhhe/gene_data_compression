# gene_data_compression

天池大赛2016年的基因碱基对的数据压缩大赛，这是我的参赛代码（排名3rd），时隔快7年翻出来归档在此。

赛题介绍：[./doc/problem_description.md](doc/problem_description.md)

算法说明：[./doc/algorithm_description.docx](doc/algorithm_description.docx)

数据：原始数据较大并没有存留，保留了一个较小的测试例，在 [./data](./data)中。

运行测试例：[./run_test.sh](./run_test.sh)，将在当前目录中编译、安装、运行测试例文件。生成的结果和中间文件在 `tmp` 目录。

输出如下：
```bash
c compress ...
c uncompress ...
c diff original data and uncompressed data ...
c END
```
