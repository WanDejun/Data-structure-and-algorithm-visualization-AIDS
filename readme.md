## 数据结构与算法可视化辅助程序(Data structure and algorithm visualization AIDS) ##

### 主界面 ###
- 采用ui进行跳转至每个子模块
- 窗口分辨率采用 1280 * 720px

### 排序部分 ###
- 冒泡
- 插入
- 选择
- 快排
- 堆排序(与堆数据结构一起做)
- 二分查找(与树的数据结构一起做)

### 数据结构部分 ###
- 链表(插入，删除)
- 栈(push, pop) (最简单)
- 队列(push, pop)
- 树(二分查找， 遍历)

### 主要开发思想 ###
- 将画面中的每个图形抽象成一组结构体数组(没有oop的悲哀)， 通过修改结构体的某个元素即可完成对画面的修改
- 每个功能定义function_main()函数用于控制算法过程，和function_draw()函数用于控制绘图

### 主要函数模块 ###
- 绘制箭头 箭头结构体数组(draw_arrow.h)
- 控制文本输出 文本结构体数组(textout.h)
- (可选，视情况开发)数据结构执行过程中由用户输入控制执行流(原本为随机数输入)，开发输入控制台
- 排序的交换可视化模块