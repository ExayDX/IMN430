//
//  Tree.h
//  IMN430_tp1
//
//  Created by Steven Aubertin on 2014-05-19.
//

#ifndef IMN430_tp1_Tree_h
#define IMN430_tp1_Tree_h

template<class EdgeT, /*class PoinT,*/ class EventT>
class TreeNode{
public:

	//---- Members
	//PoinT*	  mSite;
	EdgeT*	  mEdge;
	EventT*	  mEvent;

    //----Constructors
    TreeNode()
<<<<<<< HEAD
        : /*mSite(nullptr),*/ mEdge(nullptr), mEvent(nullptr){}

    //TreeNode(PoinT* pt)
    //    : mSite(pt), mEdge(nullptr), mEvent(nullptr){}
    
    //---- Accessors
 //   double X()const { return mSite->x;}
 //  
	//double Y()const { return mSite->y;}
 //   
	//double Z()const { return mSite->z;}
    
    void setEvent (EventT* e) { this->mEvent = e;}
    
	//---- CompareFunction
  //  struct Compare{
  //      bool operator()(const TreeNode v1, const TreeNode v2)
		//{
  //          return v1.mSite->x < v2.mSite->x;
  //      }
  //  };
=======
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
>>>>>>> dc9afffdc4133dc7f55576c30ed1efbfc2cecc1c
};

#endif
