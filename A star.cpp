#include<stdio.h>

typedef struct Point
{
	int x;
	int y;
	int sr_distance;
} Point;

typedef struct Node
{
	Point point;
	int weight;
}Node;

typedef struct heap
{
	int size;
	Node array[100];
}heap;

void heapify(heap* h, int i, int(*position)[10])
{
	Node temp = h->array[i];
	while (2 * i + 1 < h->size)
	{
		int s = 2 * i + 1;
		int min = h->array[s].weight;
		if (s + 1 < h->size && h->array[s + 1].weight < min)
		{
			min = h->array[s].weight;
			s++;
		}
		if (min < h->array[i].weight)
		{
			position[h->array[s].point.x][h->array[s].point.y] = i;
			h->array[i] = h->array[s];
			i = s;
		}
		else
			break;
	}
	position[temp.point.x][temp.point.y] = i;
	h->array[i] = temp;
}

Node heap_extract_min(heap* h, int(*position)[10])
{
	Node rt = h->array[0];
	h->array[0] = h->array[--h->size];
	heapify(h, 0, position);
	return rt;
}

void  heap_decrease_key(heap* h, int i, int key, int(*position)[10])
{
	h->array[i].weight = key;
	Node temp = h->array[i];
	while (i)
	{
		int parent = (i + 1) / 2 - 1;
		if (h->array[parent].weight > temp.weight)
		{
			position[h->array[parent].point.x][h->array[parent].point.y] = i;
			h->array[i] = h->array[parent];
			i = parent;
		}
		else
			break;
	}
	position[temp.point.x][temp.point.y] = i;
	h->array[i] = temp;
}

void  heap_insert(heap* h, Node n, int(*position)[10])
{
	h->array[h->size] = n;
	int i = h->size++;
	Node temp = h->array[i];
	while (i)
	{
		int parent = (i + 1) / 2 - 1;
		if (h->array[parent].weight > temp.weight)
		{
			position[h->array[parent].point.x][h->array[parent].point.y] = i;
			h->array[i] = h->array[parent];
			i = parent;
		}
		else
			break;
	}
	position[temp.point.x][temp.point.y] = i;
	h->array[i] = temp;
}

void print(Point(*pre)[10], Point now, Point start, char(*map)[11])
{
	if (now.x != start.x || now.y != start.y)
	{
		print(pre, pre[now.x][now.y], start, map);
		map[now.x][now.y] = '@';
	}
	else
		map[now.x][now.y] = '@';
}

int Manhattan_Distance(int x1, int y1, int x2, int y2)
{
	int x = x1 > x2 ? x1 - x2 : x2 - x1;
	int y = y1 > y2 ? y1 - y2 : y2 - y1;
	return (x + y) * 10;
}

int main()
{
	//                                上    右上   右    右下  下    左下  左     左上
	int direction[8][2] = { {-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1}, {0,-1},{-1,-1} };
	char map[10][11] =
	{
		"00000000#0",
		"00000000#0",
		"00000#00#0",
		"00000#00#0",
		"00000#0000",
		"00000#0000",
		"00000#0#00",
		"00000#0#00",
		"0000000#00",
		"0000000000"
	};
	for (int i = 0; i != 10; i++)
	{
		for (int j = 0; j != 11; j++)
			printf("%c", map[i][j]);
		printf("\n");
	}
	Point pre[10][10];
	int openset[10][10] = { 0 };
	int closeset[10][10] = { 0 };
	int positon[10][10] = { 0 };
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	printf("输入起点坐标:");
	scanf_s("%d %d", &start_x, &start_y);
	printf("输入终点坐标:");
	scanf_s("%d %d", &end_x, &end_y);
	Point start = { start_x,start_y,0 };
	Point end = { end_x,end_y };
	heap h;
	h.size = 0;
	Node temp = { start,start.sr_distance + Manhattan_Distance(start.x,start.y,end.x,end.y) };
	heap_insert(&h, temp, positon);
	openset[start.x][start.y] = 1;
	int flag = 1;
	while (closeset[end.x][end.y] == 0)
	{
		if (h.size == 0)
		{
			flag = 0;
			printf("起点无法到达终点!\n");
			break;
		}
		Node node_current = heap_extract_min(&h, positon);
		Point point_current = node_current.point;
		closeset[point_current.x][point_current.y] = 1;
		for (int i = 0; i != 8; i++)
		{
			int x = point_current.x + direction[i][0];
			int y = point_current.y + direction[i][1];
			if (x < 0 || x >= 10 || y < 0 || y >= 10)
				continue;
			if (map[x][y] == '#' || closeset[x][y])
				continue;
			else
			{
				int distance = direction[i][0] && direction[i][1] ? 14 : 10;
				if (openset[x][y])
				{
					int pos = positon[x][y];
					if (point_current.sr_distance + distance < h.array[pos].point.sr_distance)
					{
						h.array[pos].point.sr_distance = point_current.sr_distance + distance;
						heap_decrease_key(&h, pos, node_current.weight + distance + Manhattan_Distance(x, y, end.x, end.y), positon);
						pre[x][y] = point_current;
					}
				}
				else
				{
					Node n = { {x,y,point_current.sr_distance + distance}, point_current.sr_distance + distance + Manhattan_Distance(x, y ,end.x,end.y) };
					heap_insert(&h, n, positon);
					openset[x][y] = 1;
					pre[x][y] = point_current;
				}
			}
		}
	}
	if (flag)
	{
		print(pre, end, start, map);
		for (int i = 0; i != 10; i++)
		{
			for (int j = 0; j != 11; j++)
				printf("%c", map[i][j]);
			printf("\n");
		}
	}
	return 0;
}