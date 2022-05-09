#include "board.h"
#include<candyInfo.h>
#include<QPropertyAnimation>
#include<QParallelAnimationGroup>


Board::Board()
{
    setFlag(QQuickItem::ItemHasContents);
    rectNode.setColor(QColor::fromRgbF(0,0,1,0.3));

    setAcceptedMouseButtons(Qt::AllButtons);
    setClip(true);
    CandyItem * b;

    size = 67;

    for(int i=0;i<10; i++){ //a
        QList<CandyItem *> items;
        for(int j=0 ; j<10; j++ ){
            b= new CandyItem(CandyInfo::randomCandy(CandyInfo::Original), QPointF(size*i ,size*j),size);
            b->setParentItem(this);
            items.append(b);
        }
        candyItems.append(items);
    }

}


QSGNode *Board::updatePaintNode(QSGNode *rootNode, UpdatePaintNodeData *updateData)
{
    Q_UNUSED(updateData);
    if(!rootNode){
        rootNode = new QSGNode;
    }

    if(isCandySelected)rootNode->appendChildNode(&rectNode);
    else rootNode->removeAllChildNodes();
    return rootNode;
}



bool Board::scan(bool doAction){

    setSelectCandy(false);

    for(int i=0;i<10; i++){
        QList<CandyItem *> sameItem;
        for(int j=0 ; j<10 ; j++ ){
            sameItem.append(candyItems[j][i]);
            if(sameItem.size()==3){
                CandyItem * item1 = sameItem.first();
                CandyItem * item2 = sameItem.at(1);
                CandyItem * item3 = sameItem.last();
                int firstItemColor = item1->candyInfo.color;
                int secondItemColor = item2->candyInfo.color;
                int thirdItemColor = item3->candyInfo.color;

                if(firstItemColor == secondItemColor && secondItemColor == thirdItemColor)
                {
                    //qDebug()<<firstItemColor<<" "<<secondItemColor<<" "<<thirdItemColor;
                    if(doAction)
                    deleteMultipleHorizontalCandy(i,j-2,j);
                    return true;

                }
                else sameItem.removeAt(0);

            }
        }
    }

    for(int j=0 ; j<10 ; j++ ){
        QList<CandyItem *> sameItem;
        for(int i=0;i<10; i++){
            sameItem.append(candyItems[j][i]);
            if(sameItem.size()==3){
                CandyItem * item1 = sameItem.first();
                CandyItem * item2 = sameItem.at(1);
                CandyItem * item3 = sameItem.last();
                int firstItemColor = item1->candyInfo.color;
                int secondItemColor = item2->candyInfo.color;
                int thirdItemColor = item3->candyInfo.color;

                if(firstItemColor == secondItemColor && secondItemColor == thirdItemColor)
                {
                    //qDebug()<<firstItemColor<<" "<<secondItemColor<<" "<<thirdItemColor;
                     if(doAction)
                    deleteMultipleVerticalCandy(j,i-2,i);
                     return true;

                }
                else sameItem.removeAt(0);

            }
        }
    }

    return false;
}

void Board::mousePressEvent(QMouseEvent *event)
{
    auto p = event->pos();
    CandyItem  *  item = dynamic_cast<CandyItem *>(childAt(p.x(),p.y()));
    if(item!=nullptr){
        onCandyClicked(item);
    }
}

void Board::setSelectCandy(bool select, CandyItem *candy)
{
    if(!select){
        isCandySelected = false;
        update();
    }
    else{
        if(candy!=nullptr){
            isCandySelected = true;
            selectedCandy = candy;

            auto rect = candy->boundingRect();
            rect.moveTopLeft(candy->position());
            rect = rect-QMarginsF(3,3,3,3);
            rectNode.setRect(rect);
            update();

        }
    }
}


void Board::onCandyClicked(CandyItem * currentCandy){

    if(!isCandySelected) {
        setSelectCandy(true,currentCandy);
    }
    else
    {
        setSelectCandy(false);

        if(selectedCandy!= currentCandy){

            if(selectedCandy->row() - currentCandy->row()== 0
                    && qAbs(selectedCandy->column() - currentCandy->column() )==1){
               // qDebug()<<"Horizontal swap";
                swap(selectedCandy,currentCandy,true);

            }
            else  if( selectedCandy->column() - currentCandy->column()==0
                      && qAbs(selectedCandy->row() - currentCandy->row())==1)
            {
               // qDebug()<<"Vertical swap";
                swap(selectedCandy,currentCandy,false);

            }
        }
    }
}

void Board::deleteSingleCandy(CandyItem * candy)
{
    int column = candy->column();
    int row = candy->row();

    candyItems[column].removeAt(row);
    candy->setPosition(QPointF(size*column ,-size));
    candy->setCandy(CandyInfo::randomCandy(CandyInfo::Original));
    candyItems[column].prepend(candy);

    QParallelAnimationGroup * groupAnimation = new QParallelAnimationGroup;
    for( int i=0 ; i<=row ; i++){
        CandyItem * item = candyItems[column].at(i);
        QPropertyAnimation * animation = new QPropertyAnimation(item,"y");
        animation->setDuration(duration);
        animation->setStartValue(item->y());
        animation->setEndValue(item->y()+size);
        groupAnimation->addAnimation(animation);
    }

    QObject::connect(groupAnimation,&QParallelAnimationGroup::finished,this,[this](){ scan(); });
    groupAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Board::deleteMultipleVerticalCandy(int column, int from, int to)
{
    QList<CandyItem *> itemToRemove;
    int index =1;
    for(int i=from; i<=to ; i++ ){
        CandyItem * candy = candyItems[column].at(i);
        candy->setPosition(QPointF(size*column ,-size*index));
        candy->setCandy(CandyInfo::randomCandy(CandyInfo::Original));
        itemToRemove.append(candy);
        index++;
    }

    for(int i=from; i<=to ; i++ )
        candyItems[column].removeAt(from);


    index=0;
    for(int i=from; i<=to ; i++ ){
        candyItems[column].prepend(itemToRemove.at(index));
        index++;
    }


    QParallelAnimationGroup * groupAnimation = new QParallelAnimationGroup;
    groupAnimation->clear();
    CandyItem * item;
    float moveIntoY = size*(to-from+1);
    for( int i=0 ; i<=to ; i++){
        item = candyItems[column].at(i);
        QPropertyAnimation * animation = new QPropertyAnimation(item,"y");
        animation->setDuration(duration);
        animation->setStartValue(item->y());
        animation->setEndValue(item->y()+moveIntoY);
        groupAnimation->addAnimation(animation);
    }

    QObject::connect(groupAnimation,&QParallelAnimationGroup::finished,this,[this](){ scan(); });
    groupAnimation->start(QAbstractAnimation::DeleteWhenStopped);

}

void Board::deleteMultipleHorizontalCandy(int row, int from, int to)
{
    for(int column=from; column<=to; column++)
    {
        CandyItem * candy = candyItems[column].at(row);
        candyItems[column].removeAt(row);
        candy->setPosition(QPointF(size*column ,-size));
        candy->setCandy(CandyInfo::randomCandy(CandyInfo::Original));
        candyItems[column].prepend(candy);
    }

    QParallelAnimationGroup * groupHAnimation = new QParallelAnimationGroup;
    for(int column=from; column<=to; column++){

        QParallelAnimationGroup * groupVAnimation = new QParallelAnimationGroup;
        for(int i=0;i<=row;i++){
            CandyItem * item = candyItems[column].at(i);
            QPropertyAnimation * animation = new QPropertyAnimation(item,"y");
            animation->setDuration(duration);
            animation->setStartValue(item->y());
            animation->setEndValue(item->y()+size);
            groupVAnimation->addAnimation(animation);
        }
        groupHAnimation->addAnimation(groupVAnimation);
    }

    QObject::connect(groupHAnimation,&QParallelAnimationGroup::finished,this,[this](){ scan(); });
    groupHAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}


void Board::swap(CandyItem * firstItem, CandyItem * secondItem, bool Horizontal)
{
    candyItems[firstItem->column()][firstItem->row()] = secondItem;
    candyItems[secondItem->column()][secondItem->row()]  = firstItem;
    if(!scan(false))
    {
        candyItems[firstItem->column()][firstItem->row()] = firstItem;
        candyItems[secondItem->column()][secondItem->row()]  = secondItem;
        return;
    }

    QParallelAnimationGroup * groupAnimation = new QParallelAnimationGroup;
    QPropertyAnimation * anim1 = new QPropertyAnimation(firstItem,"");
    QPropertyAnimation * anim2 = new QPropertyAnimation(secondItem,"");
    anim1->setDuration(duration);
    anim2->setDuration(duration);

    if(Horizontal){

        anim1->setPropertyName("x");
        anim1->setStartValue(firstItem->x());
        anim1->setEndValue(secondItem->x());

        anim2->setPropertyName("x");
        anim2->setStartValue(secondItem->x());
        anim2->setEndValue(firstItem->x());

    }
    else
    {
        anim1->setPropertyName("y");
        anim1->setStartValue(firstItem->y());
        anim1->setEndValue(secondItem->y());

        anim2->setPropertyName("y");
        anim2->setStartValue(secondItem->y());
        anim2->setEndValue(firstItem->y());

    }

    groupAnimation->addAnimation(anim1);
    groupAnimation->addAnimation(anim2);

    QObject::connect(groupAnimation,&QParallelAnimationGroup::finished,this,[this](){ scan(); });
    groupAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
