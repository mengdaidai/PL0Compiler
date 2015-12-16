#ifndef NODELIST_H__
#define NODELIST_H__


typedef struct node
{
    //ѭ��˫�����Ľ��ṹ
    void* data;//������ָ��
    struct node *next;//ָ��ǰ������һ���
    struct node *last;//ָ��ǰ������һ���
}Node;

struct list
{
    struct node *head;//ͷָ�룬ָ��ͷ���
    int data_size;//������Ӧ��������ռ�ڴ�Ĵ�С
    int length;//����list�ĳ���
};







//����ѭ��˫��������ͷ��㣬����±��0��ʼ��ͷ��㲻�����±�ֵ

//������ָ��Node*ΪList���͵ĵ�����
typedef struct node* Iterator;

//List���͵Ķ���
typedef struct list* List;


static Node* Position(List list, int index);
static Node* NewNode(int data_size);
static void LinkNodeToList(List list, Node *node, Node *next_node);
int RemoveNode(List list, Node *node);

//��ʼ������,��������ռ�ڴ�Ĵ�С��data_size����
int InitList(List *list, int data_size);

//��data�����ݲ��뵽����list��ĩβ
//assignָ������data��ĸ�ֵ����
Iterator Append(List list, void *data,
                void (*assign)(void*, const void*));

//��data�����ݲ��뵽�����ĵ�����it_before��ǰ��
//assignָ������data��ĸ�ֵ����
Iterator Insert(List list, void *data, Iterator it_before,
                void (*assign)(void*, const void*));

//������A�е�����it_aָ��Ľ���ƶ�������B�е�����it_b_befroe��ǰ��
Iterator MoveFromAtoB(List A, Iterator it_a,
                      List B, Iterator it_b_before);

//ɾ������list�е�����itָ��Ľ��
int Remove(List list, Iterator it);

//ɾ������list�ĵ�0����㣬�±��0��ʼ
int RemoveFirst(List list);

//ɾ������list�����һ�����
int RemoveLast(List list);

//����list�е�index�����ݵ�ָ��
void* At(List list, int index);

//��begin��end֮����ҷ���condition�ĵ�һ��Ԫ�أ�
//�ȽϺ�����conditionָ��,�Ƚϵ�ֵ��dataָ��
//����һ��������ֵС�ڵڶ���������ֵʱ������1�����򷵻�0
//����condition�����Ĳ�ͬ�����Բ��ҵ�һ����ȡ����ڻ�С��data��ֵ
Iterator FindFirst(Iterator begin, Iterator end, void *data,
                       int (*condition)(const void*, const void*));

//����list�е�һ����data��ȵ�Ԫ�ص��±꣬
//equal����������һ��������ڶ���������ֵ���ʱ������1�����򷵻�0
int IndexOf(List list, void *data,
            int (*equal)(const void*,const void*));

//������begin��end֮�����Сֵ���ȽϺ�����lessָ��
//����һ��������ֵС�ڵڶ���������ֵʱ������1�����򷵻�0
Iterator GetMin(Iterator begin, Iterator end,
                int (*less)(const void*, const void*));

//������begin��end֮������ֵ���ȽϺ�����largeָ��
//����һ��������ֵ���ڵڶ���������ֵʱ������1�����򷵻�0
Iterator GetMax(Iterator begin, Iterator end,
                int (*large)(const void*, const void*));

//��ȡlist�ĳ���
int GetLength(List list);
//��listΪ���������򷵻�1�����򷵻�0
int IsEmpty(List list);
//����list
void DestroyList(List *list);

//�����������
void printList(List list);

//���list���׵�����
Iterator Begin(List list);

//���list��β��������ָ�����һ��Ԫ�ص���һ��λ��
Iterator End(List list);

//ʹitָ����һ��λ�ã�������ָ����һ��λ�ú�ĵ�����
Iterator Next(Iterator *it);

//ʹitָ����һ��λ�ã�������ָ����һ��λ�ú�ĵ�����
Iterator Last(Iterator *it);

//ͨ��������it������ݣ��൱��*p
void* GetData(Iterator it);

//��ȡ��ǰ����������һ����������ע�⣬�����ı䵱ǰ������
Iterator GetNext(Iterator it);

//��ȡ��ǰ����������һ����������ע�⣬�����ı䵱ǰ������
Iterator GetLast(Iterator it);




#endif // LIST_H_INCLUDED