#pragma once
//全局变量

#define MAXNUM 0x3f3f3f3f
const int n = 200; // map size
const int robot_num = 10;
const int boat_num = 5;
const int berth_num = 10;
const int random_bfs_point = 100;
int money;         // 钱数 （分数）
int boat_capacity; // 船装货上限
// 到对应泊位的距离
int berth_dis[n][n][berth_num];
// 到随机点的距离
int random_dis[n][n][random_bfs_point + 50];
// 所有随机点的坐标
vector<pair<int, int>> random_point;


int id;
/**
 * 地图
 * .空地
 * *海洋
 * #障碍
 * 0-9泊位所在的位置
 * */
char game_map[n][n];
