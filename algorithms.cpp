#include "algorithms.h"

#define BOUNDINGBOX 1e+6

Algorithms::Algorithms()
{

}

void Algorithms::inizializateDataStructures(TrapezoidalMap& tm, Dag& dag){
    /*Starting bounding box aka first trapezoid*/
    cg3::Point2d BBx1y1 = cg3::Point2d(-BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx2y1 = cg3::Point2d(BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d BBx1y2 = cg3::Point2d(-BOUNDINGBOX, -BOUNDINGBOX);
    cg3::Point2d BBx2y2 = cg3::Point2d(BOUNDINGBOX, -BOUNDINGBOX);

    cg3::Segment2d boundingBoxTop = cg3::Segment2d(BBx1y1, BBx2y1);
    cg3::Segment2d boundingBoxBottom = cg3::Segment2d(BBx1y2, BBx2y2);
    Trapezoid * firstTrapezoid = new Trapezoid(boundingBoxTop, boundingBoxBottom);
    Node * firstNode = new LeafNode(firstTrapezoid);
    firstTrapezoid->node = firstNode;
    tm.setLeftMostTrapezoid(firstTrapezoid);
    dag.setRoot(firstNode);
};

void Algorithms::BuildTrapezoidalMap(TrapezoidalMap &tm, Dag &dag, std::vector<cg3::Segment2d> segmentList){
    /*Permute segments order*/
    if (tm.getSegmentListSize() > 1){
         tm.permuteSegmentList();
    }
    for (size_t i = 0; i < tm.getSegmentListSize(); i++){
        BuildTrapezoidalMap(tm, dag, segmentList[i]);
    }

}

void Algorithms::BuildTrapezoidalMap(TrapezoidalMap& tm, Dag& dag, cg3::Segment2d segment){

    /*Validate segments*/
    geoutils::validateSegment(segment);

    std::vector<Trapezoid*> trapezoidsIntersected =  FollowSegment(dag, segment);

    /*check: understand if is needed to keep and order in tm*/
    if (trapezoidsIntersected.size() == 1){
        subgraphFromOneTrapezoid(tm, dag, trapezoidsIntersected[0], segment);
        tm.removeTrapezoid(trapezoidsIntersected[0]);
    }else{

    }

};

/*

Follow Segment

*/

std::vector<Trapezoid*> Algorithms::FollowSegment(Dag& dag, cg3::Segment2d s1){
    cg3::Point2d p1 = s1.p1();
    cg3::Point2d q1 = s1.p2();

    std::vector<Trapezoid*> trapezoidsIntersected = std::vector<Trapezoid*>();
    Trapezoid* delta0 = dag.QueryPoint(dag.root, p1);

    trapezoidsIntersected.push_back(delta0);
    int j = 0;
    while (q1.x() > trapezoidsIntersected[j]->getRightp().x()){
        if (geoutils::isAbove(trapezoidsIntersected[j]->getRightp(), s1)){
           trapezoidsIntersected.push_back(trapezoidsIntersected[j]->getLowerRightNeighbor());

        }else{
            trapezoidsIntersected.push_back(trapezoidsIntersected[j]->getUpperRightNeighbor());
        }
        j++;
    }
    return trapezoidsIntersected;

}


void Algorithms::subgraphFromOneTrapezoid(TrapezoidalMap& tm, Dag& dag, Trapezoid* t, cg3::Segment2d s){
    XNode *p1 = new XNode(s.p1());
    XNode *q1 = new XNode(s.p2());
    YNode *s1 = new YNode(s);
    std::vector<Trapezoid*> splitResult = t->SplitTrapezoid(s);

    p1->setLeft(new LeafNode(splitResult[0]));
    tm.addTrapezoid(splitResult[0]);

    s1->setLeft(new LeafNode(splitResult[1]));
     tm.addTrapezoid(splitResult[1]);

    s1->setRight(new LeafNode(splitResult[2]));
     tm.addTrapezoid(splitResult[2]);

    q1->setRight(new LeafNode(splitResult[3]));
     tm.addTrapezoid(splitResult[3]);

    q1->setLeft(s1);
    p1->setRight(q1);

    dag.substituteTargetNode(dag.root, t->node, p1); //logn
    //t->node = p1; //non funziona per scope o(1)
}

void Algorithms::addTrapezoids(Node* root, TrapezoidalMap& tm){
    if (root == nullptr or root->getType() == leafNode){
        return;
    }else{
        if(root->left == nullptr and root->right != nullptr){
            createAndInsertLeaves(root, tm, true);
            addTrapezoids(root->right, tm);
        }
        if(root->right == nullptr and root->left != nullptr){
            createAndInsertLeaves(root, tm, false);
            addTrapezoids(root->left, tm);
        }
        if(root->right == nullptr and root->left == nullptr){
            createAndInsertLeaves(root,tm, true);
            createAndInsertLeaves(root,tm, false);
        }
        if(root->right != nullptr and root->left != nullptr){
            addTrapezoids(root->left, tm);
            addTrapezoids(root->right, tm);
        }
    }
};

void Algorithms::createAndInsertLeaves(Node * node, TrapezoidalMap& tm, bool leftChild){
    if (node->getType() == xNode){
        XNode * x = (XNode*)node;
        Trapezoid* delta = new Trapezoid(x->getPoint(), leftChild);
        Node * leaf = new LeafNode(delta);
        delta->node = leaf;
        if (leftChild){
             node->setLeft(leaf);
             tm.addTrapezoid(delta);
        }else{
            node->setRight(leaf);
            tm.addTrapezoid(delta);
        }

    }
    if (node->getType() == yNode){
        YNode * y = (YNode*)node;
        Trapezoid *delta = new Trapezoid(y->getSegment(), leftChild); // se è a sx di un segmento è above
        Node * leaf = new LeafNode(delta);
                delta->node = leaf;
        if (leftChild){
             node->setLeft(leaf);
             tm.addTrapezoid(delta);
        }else{
            node->setRight(leaf);
            tm.addTrapezoid(delta);
        }
    }
    if(node->getType() == leafNode){
        return;
    }
}




