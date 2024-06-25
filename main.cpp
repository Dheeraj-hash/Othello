#include<iostream>
#include<set>
#include<string>
using namespace std;
class coord{
    
    public:
    int x,y;
    coord(int a,int b){
        x = a;
        y = b;
    }
    coord operator +(coord next){
        next.x+=x;next.y+=y;
        return next;
    }
    coord operator -(coord next){
        next.x= x-next.x;next.y = y-next.y;
        return next;
    }
    void operator =(coord next){
        x=next.x,y=next.y;
    }
    bool operator ==(coord next)const{
        return (next.x==x && next.y==y);
    }
    bool operator <(coord next)const{
        if(x<next.x){return true;}
        if(x==next.x){return y<next.y;}
        return false;
    }
    bool valid(){
        if(x >= 0 && x<8 && y>=0 && y<8){
            return true;
        }
        return false;
    }
    

};
class othello{
    
    private:
    char board[8][8];
    set<coord> opp,me,oppavail,meavail;
    
        
    public:
    
    othello(){
        //initialise display board
        for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
                board[x][y]=' ';
            }
        }
        
        me.insert(coord(3,3));opp.insert(coord(3,4));
        opp.insert(coord(4,3));me.insert(coord(4,4));

        displayboard();
        oppturn();    
    }

    void oppturn(){
        availrefresh();
        if(oppavail.empty()){meturn();}
        update(oppmoveread());
        if(check()){cout << check();displayboard();exit(0);}
        availrefresh();
        displayboard();
        meturn();       
    }

    void meturn(){ 
        // print();
        if(meavail.empty()){oppturn();}
        coord x = bestmove();
        me.insert(x);
        update(x);
        if(check()){cout << check();displayboard();exit(0);}
        availrefresh();
        displayboard();
        oppturn();
    }

    void displayboard(){
        
        // refreshing display board
        for(coord opppos:opp){
            board[opppos.x][opppos.y]='X';
        }
        for(coord mepos:me){
            board[mepos.x][mepos.y]='Y';            
        }

        cout << "  0   1   2   3   4   5   6   7\n";
        for(int x=0;x<8;x++){
            
            cout << " ";
            for(int y=0;y<7;y++){
                cout << "   |";
            }
            cout << "   \n";
            
            cout << x;
            for(int y=0;y<7;y++){
                cout << " " << board[x][y] << " |"; 
            }
            cout << " " << board[x][7] << " \n";
            
            cout << " ";
            for(int y=0;y<7;y++){
                cout << "   |";
            }
            cout << "   \n";
            
            if(x<7){
            cout << " ";
            for(int y=0;y<10;y++){
                cout << "___";
            }
            cout << "_\n";
            }

        }
    }

    int check(){
        if(oppavail.size() == 0 && meavail.size() == 0){
            if(opp.size()>me.size()){return -1;}
            return 1;    
        }        
        return 0;
    }

    coord bestmove(){
        int max=-100000000;
        coord bestpos(0,0);
        int size=meavail.size();
        for(int i=0;i<size;i++){
            auto temp = meavail.begin();
                for(int j=0;j<i;j++){
                    temp++;
                }
                coord pos = *temp;
            int tempres = resme(pos,0);
            if(tempres>=max){max=tempres;bestpos=pos;}
        }
        return bestpos;
    } 
    //for me
    int resme(coord pos,int depth){
        depth++;
        // cout << depth;
        set<coord> parentme = me,parentopp = opp;
        me.insert(pos);
        update(pos);
        availrefresh();
        // print();
        if(check()){
        
        me=parentme;opp=parentopp;availrefresh();return check()*100000;}
        if(depth == 5){
        me=parentme;opp=parentopp;availrefresh();return (int)me.size() - (int)opp.size();}

        if(oppavail.size()==0){
            set<int> maxrespos;
            int size = meavail.size();
            for(int i=0;i<size;i++){
                auto temp = meavail.begin();
                for(int j=0;j<i;j++){
                    temp++;
                }
                coord child = *temp;
                maxrespos.insert(resme(child,depth));
            }
            me=parentme;opp=parentopp;availrefresh();
            return *maxrespos.rbegin();
        }
        set<int> minrespos;
        int size = oppavail.size();
        for(int i=0;i<size;i++){
            auto temp = oppavail.begin();
                for(int j=0;j<i;j++){
                    temp++;
                }
                coord child = *temp;
            minrespos.insert(resopp(child,depth));
        }
        me=parentme;opp=parentopp;availrefresh();
        return *minrespos.begin();
    }

    int resopp(coord state,int depth){
        depth++;
        // cout << depth;
        set<coord> parentme=me,parentopp=opp;
        opp.insert(state);
        update(state);
        availrefresh();
        if(check()){me=parentme;opp=parentopp;availrefresh();return check()*100000;}
        if(depth == 5){   
        me=parentme;opp=parentopp;availrefresh();return (int)me.size() - (int)opp.size();}
        if(meavail.size()==0){
            set<int> minrespos;
            int size=oppavail.size();
            for(int i=0;i<size;i++){
                auto temp = oppavail.begin();
                for(int j=0;j<i;j++){
                    temp++;
                }
                coord child = *temp;
                minrespos.insert(resopp(child,depth));
            }
            me=parentme;opp=parentopp;availrefresh();
            return *minrespos.begin();
        }
        set<int> maxrespos;
        int size = meavail.size();
        for(int i=0;i<size;i++){
            auto temp = meavail.begin();
            for(int j=0;j<i;j++){
                temp++;
            }
            coord child = *temp;
            maxrespos.insert(resme(child,depth));
        }
        me=parentme;opp=parentopp;availrefresh();
        return *maxrespos.rbegin();
    }


    coord oppmoveread(){
        string pos;
        cout << "Enter your move: x-y \n";
        cin >> pos;
        int x = pos[0] - '0';
        int y= pos[2] - '0';
        if(oppavail.count(coord(x,y))){           
            opp.insert(coord(x,y));
            return coord(x,y);
        }
        return oppmoveread();
    }


    void availrefresh(){
        oppavail.clear();
        for(auto start: opp){
            for(int x=-1;x<2;x++){
                for(int y=-1;y<2;y++){
                    coord temp = start;
                    if(me.count(start+coord(x,y))){
                        for(int i=0;i<8;i++){
                            temp = temp + coord(x,y);
                            if(!me.count(temp)){
                                if(opp.count(temp)){break;}
                                if(temp.valid()){oppavail.insert(temp);break;}
                            }
                        }
                    }
                }
            }
            
        }
        meavail.clear();
        for(auto start: me){
            for(int x=-1;x<2;x++){
                for(int y=-1;y<2;y++){
                    coord temp = start;
                    if(opp.count(start+coord(x,y))){
                        for(int i=0;i<8;i++){
                            temp = temp + coord(x,y);
                            if(!opp.count(temp)){
                                if(me.count(temp)){break;}
                                if(temp.valid()){meavail.insert(temp);break;}
                            }
                        }
                    }
                }
            }
            
        }
    }


    void update(coord latest){
       for(int x=-1;x<2;x++){
        for(int y=-1;y<2;y++){
            flip(latest,coord(x,y));
        }
       }    
    }


    void flip(coord latest, coord dir){
        string player; set<coord> type,opptype;
        if (opp.count(latest)){
            player = "opp";
            type = opp,opptype = me;
        }
        else{
            player = "me";
            type = me,opptype = opp;
        }

        if(opptype.count(latest+dir)){
            for(int i=0;i<8;i++){
                latest = latest+dir;
                if(!opptype.count(latest)){
                    if(type.count(latest)){
                        for(int j=0;j<8;j++){
                            latest = latest-dir;
                            if(type.count(latest)){return;}
                            if(player == "opp"){opp.insert(latest);me.erase(latest);}
                            else{ me.insert(latest);opp.erase(latest);}

                        }
                    return;
                    }
                }
            }
        }
    }

    void print(){
        cout << "me:{";
        for(auto it:me){
            cout << '(' << it.x <<',' << it.y << ')' << ',';
        }
        cout << "}\n";

        cout << "opp:{";
        for(auto it:opp){
            cout << '(' << it.x <<',' << it.y << ')' << ',';
        }
        cout << "}\n";
    }
};

int main(){
    othello start;
}