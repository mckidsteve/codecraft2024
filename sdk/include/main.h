#pragma once
//ȫ�ֱ���

#define MAXNUM 0x3f3f3f3f
const int n = 200; // map size
const int robot_num = 10;
const int boat_num = 5;
const int berth_num = 10;
const int random_bfs_point = 100;
int money;         // Ǯ�� ��������
int boat_capacity; // ��װ������
// ����Ӧ��λ�ľ���
int berth_dis[n][n][berth_num];
// �������ľ���
int random_dis[n][n][random_bfs_point + 50];
// ��������������
vector<pair<int, int>> random_point;


int id;
/**
 * ��ͼ
 * .�յ�
 * *����
 * #�ϰ�
 * 0-9��λ���ڵ�λ��
 * */
char game_map[n][n];
