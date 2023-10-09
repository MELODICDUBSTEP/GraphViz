#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include "SimpleGraph.h"


//extension 1：更改参数
//发现参数设置太小了就会出问题
//想找一组比较好地解决64wheel的，但没找到
const double k_repel =1e-3;
const double k_attract =1e-3;

//const double k_repel =9e-4;
//const double k_attract =9e-4;

//const double k_repel =5e-4;
//const double k_attract =9e-3;

//const double k_repel =9e-4;
//const double k_attract =3e-3;

const double Pi = 3.14159265358979323;
//三个常数

using namespace std;

void Welcome();

// Main method

//由于我是编程零基础，所以在做本次作业之前还恶补了一下文件操作和vector相关的知识，
//不过由于类与对象等知识都没学，所以理解不是很深刻，只是照葫芦画瓢使用fstream和vector的功能
//还有就是，第一次写的时候一直显示不出来终端窗口，查了下博客才知道要勾选“左侧项目->run->run in terminal”

/* 以下内容为边写边参考防止弄混几个类型
 struct Node
 {
   double x, y;
 };
Node是一个结构体，存位置坐标

struct Edge
 {
  std::size_t start, end;
};
Edge是一个结构体，存两个顶点

struct SimpleGraph
{
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};
整个图是一个结构体，有两个vector作为成员，一个是以各结点为元素的nodes(区分这里定义的Node与nodes)
另一个是以各边为元素的edges（同样区分）
*/
int main()
{
    Welcome();
    //引导语
    // Task 1:读入
    cout << "please type in the file path of your file" <<endl;
    string the_file_path;
    cin >> the_file_path;
    //让用户输入路径，实际上这里输入的是文件名称
    ifstream fs;
    fs.open( the_file_path ,ios::in);
    //用流对象的成员函数打开文件用于读入数据
    SimpleGraph graph;
    //创建出存东西的图
    size_t num;
    fs >> num;
     //把节点个数读进来
    while( true )
    {
        size_t node1,node2;
        fs>>node1>>node2;
        Edge temp = { node1 ,node2 };
        //读到一条边的两个节点，先临时存起来，再加到自己的图的edges 这个vector中
        graph.edges.push_back( temp );
        if( fs.eof() )
        {
            break;
        }//没读到文件尾就一直一行一行读
    }
    //  cout<<graph.edges.size();
    // 用于检查的代码
    //读完了
    graph.nodes.resize(num);
    //用resize设置nodes的大小，为那些元素分配好内存
    fs.close();
    //关闭文件的好习惯

     //以上是task 1:读入步骤

    InitGraphVisualizer( graph );
   //按照教程，用这个函数
    //Task 2：初始化

    //思路：把一个圆均分num份，然后定每个点的初始位置
    for(size_t k = 0 ; k < num ; k++ )
    {
        graph.nodes[k].x = cos ( k * 2*Pi/ num );
        graph.nodes[k].y = sin ( k * 2*Pi/ num );
    }
    //我第一次写这里的时候，右边函数的参数写成了 k/ num *2 * Pi,那样的话就会导致一个整数除法，k/num必然会等于0，那样就出错了，这个错误找了好久才发现QAQ
    // 似乎用迭代器或者for_each来初始化还麻烦一些，所以不如就用下标的方式

    //Task 3: 开始几轮位置改变的循环,其中包含了控制运行时长的步骤
    cout<<"how many seconds do you want this program to run?"<<endl;
    size_t seconds;
    cin>>seconds;
    //读入用户想跑多少秒
    time_t startTime = time(NULL);
    //记录初始时间，这行是教程里给的


    while( difftime(time(NULL), startTime)<= seconds )
        //直到已经过了用户指定的秒数，那就停下来，经查询difftime返回的单位就是秒，不用做换算
    {
        //先算斥力，斥力是任何两个结点都有的，所以两重循环
        //但是不能算完斥力就把原来的结点坐标进行改变，否则算引力的时候就位置不对了，那就再开一个vector，把斥力偏移量先存起来，等引力也算完再一起改变结点的位置
        vector <Node> offset(num , { 0.0 , 0.0 });
                //记录偏移量的vector,并赋初始值0
        //这里不能把offset定义在while外面，因为每次都要赋其中Node的俩成员赋成0.
        DrawGraph( graph );
        //先画个图
        for(size_t i = 0;i < num; i++ )
        {
            for(size_t j = i + 1; j < num ;j ++)
                //两轮循环，没有重复
            {
                double angle = atan2 (graph.nodes[j].y-graph.nodes[i].y,graph.nodes[j].x-graph.nodes[i].x);
                //用教程的公式算一波角
                double repulsion = k_repel/(sqrt(  pow(graph.nodes[i].x-graph.nodes[j].x,2) + pow(graph.nodes[i].y-graph.nodes[j].y,2)));
                //算斥力
                offset[i].x -= repulsion * cos (angle);
                offset[i].y -= repulsion * sin (angle);
                offset[j].x += repulsion * cos (angle);
                offset[j].y += repulsion * sin (angle);
                //这里加减号不能反，因为那个角的缘故
            }
         }
            //接下来算引力
            for(vector <Edge>:: iterator it = graph.edges.begin(); it!=graph.edges.end();it++)
                //试试新学的迭代器
            {
                double angle = atan2 (graph.nodes[(*it).end].y-graph.nodes[(*it).start].y,graph.nodes[(*it).end].x-graph.nodes[(*it).start].x);
                double attraction =  k_attract*(pow(graph.nodes[(*it).start].x-graph.nodes[(*it).end].x,2) + pow(graph.nodes[(*it).start].y-graph.nodes[(*it).end].y,2));
                offset[(*it).start].x += attraction * cos(angle);
                offset[(*it).start].y += attraction * sin(angle);
                offset[(*it).end].x -= attraction * cos(angle);
                offset[(*it).end].y -= attraction * sin(angle);
             }
            //都算好了，一起加
            //这里做一个extension 2：随机扰动
            //用随机数的rand函数来做
            //不过并没有解决64wheel的问题，但似乎解决了其他很多图的问题，比如127binary-tree变得更好看了
            for(size_t k = 0 ; k<num ;k++)
            {
                srand(time(0));
                graph.nodes[k].x = ( graph.nodes[k].x+(offset[k].x) )*((rand( )%10*1.0-5.0)*0.0001+1.0);
                graph.nodes[k].y = ( graph.nodes[k].y+(offset[k].y) )*((rand( )%10*1.0-5.0)*0.0001+1.0);
                //经过实践，这个扰动不能设置太大了，否则图一直大幅度震荡，随机性太强
                //这个随机扰动的参数也要根据不同图来改，有些图加了随机扰动反而不好，比如60cycle，但对于树状结构随机扰动非常管用
                //这里的随机有一点粗糙：是对坐标随机乘一个接近1的量，这样会导致效应对坐标大的点更明显
                //不加随机扰动的版本：
                //graph.nodes[k].x += offset[k].x;
                //graph.nodes[k].y += offset[k].y;


                //这里再做一个extension 5:低分辨率惩罚，希望解决一下64wheel的问题
                //但是感觉这个会使程序变慢，所以似乎有些图就没必要开低分辨率惩罚
                vector <size_t> the_nodes_with_mutual_edge ;
                //新开一个vector，用来装每个点共边的那些点

                for(vector <Edge>:: iterator it = graph.edges.begin(); it!=graph.edges.end();it++)
                {
                    if((*it).end == k )
                    {
                        the_nodes_with_mutual_edge.push_back( (*it).start );

                    }
                    else if((*it).start == k )
                    {
                        the_nodes_with_mutual_edge.push_back( (*it).end );

                    }
                    //把共边的那些点扔进去
                }
                for (size_t i = 0 ; i < the_nodes_with_mutual_edge.size(); i++ )
                {
                    for( size_t j = i + 1 ; j < the_nodes_with_mutual_edge.size() ; j++ )
                    {
                        double angle1 = atan2 (graph.nodes[j].y-graph.nodes[k].y,graph.nodes[j].x-graph.nodes[k].x);
                        double angle2 = atan2 (graph.nodes[i].y-graph.nodes[k].y,graph.nodes[i].x-graph.nodes[k].x);
                        double angle3 = fabs(angle1 - angle2);
                        if(angle3 > Pi)
                        {
                            angle3-=Pi;
                        }
                        //这里保证两边的夹角是0到pi之间的一个角
                        if( angle3 <= 0.0005*Pi )
                        {
                            double penalty = k_repel/(sqrt(  pow(graph.nodes[i].x-graph.nodes[j].x,2) + pow(graph.nodes[i].y-graph.nodes[j].y,2)));
                            double angle4 = atan2 (graph.nodes[j].y-graph.nodes[i].y,graph.nodes[j].x-graph.nodes[i].x);
                            graph.nodes[i].x -= penalty * cos (angle4);
                            graph.nodes[i].y -= penalty * sin (angle4);
                            graph.nodes[j].x -= penalty * cos (angle4);
                            graph.nodes[j].y -= penalty * sin (angle4);
                            //按照之前的思路，把惩罚作为一个斥力算进去
                        }
                    }
                }
                //开一个二重循环，计算那些边两两之间的夹角，夹角用高中学的到角公式来算，如果夹角太小，对两个顶点加以惩罚
            }
            DrawGraph( graph );
            //每一轮都画图
    }
    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome()
{
    cout << "Welcome to CS1958 GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}
