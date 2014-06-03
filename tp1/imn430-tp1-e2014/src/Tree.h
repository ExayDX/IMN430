//
//  Tree.h
//  IMN430_tp1
//
//  Created by Steven Aubertin on 2014-05-19.
//

#ifndef IMN430_tp1_Tree_h
#define IMN430_tp1_Tree_h

template<class EdgeT, class PoinT, class EventT>
class TreeNode{
public:
    //----Consrtructors
    TreeNode()
        : site(nullptr), edge(nullptr), event(nullptr){
    }
    TreeNode(PoinT* pt, EventT* ev)
        : site(pt), event(ev), edge(nullptr){
    }
    
    //---- Accessors
    double X()const{
        return site->x;
    }
    double Y()const{
        return site->y;
    }
    double Z()const{
        return site->z;
    }
    
    PoinT* getPoint()const{
        return site;
    }
    
    //---- Members
	PoinT*	  site;
	EdgeT*	  edge;//TODO: should we really put that here ????
	EventT*	  event;
    
    struct Compare : public std::binary_function<TreeNode, TreeNode, bool>{
        bool operator()(const TreeNode v1, const TreeNode v2){
            return v1.site->x < v2.site->x;
        }
    };
};

#endif
