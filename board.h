#ifndef BOARD_H
#define BOARD_H

#include<QQuickItem>
#include<candyitem.h>
#include<QSGSimpleRectNode>


class Board : public QQuickItem
{
    Q_OBJECT
public:
    Board();
    void onCandyClicked(CandyItem * currentCandy);

public slots:
    bool scan(bool doAction=true);
    void deleteMultipleVerticalCandy(int column,int from ,int to);
    void deleteMultipleHorizontalCandy(int row,int from , int to);

private:
    int duration = 200;
    float size = 70;
    bool isCandySelected = false;
    CandyItem * selectedCandy;
    QList<QList<CandyItem *> > candyItems;
    void deleteSingleCandy(CandyItem * candy);
    void swap(CandyItem * firstItem , CandyItem * secondItem ,bool Horizontal);
    QSGSimpleRectNode rectNode;
    void setSelectCandy(bool select ,CandyItem * candy=nullptr);


protected:
    void mousePressEvent(QMouseEvent * event) override;
    QSGNode *updatePaintNode(QSGNode * rootNode, UpdatePaintNodeData * updateData)override;
};

#endif // BOARD_H
