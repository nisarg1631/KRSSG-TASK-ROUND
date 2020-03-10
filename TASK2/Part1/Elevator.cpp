/*
IMPROVEMENTS

1. Updated best_call to check if there is an input at the floor where lift is initially at.
2. Updated in_way_calls to take multiple entries from same floor.
*/

#include<bits/stdc++.h>
using namespace std;

struct input{
  int start,end,id,flag;
  char direction;
};
struct user{
  int floor,id;
  char loc;
};

struct compare_up{
  bool operator()(user const& u1,user const& u2){
    return u1.floor>u2.floor;
  }
};
struct compare_down{
  bool operator()(user const& u1,user const& u2){
    return u1.floor<u2.floor;
  }
};

class lift{
  public:
    priority_queue<user, vector<user>, compare_up> route_up;
    priority_queue<user, vector<user>, compare_down> route_down;
    int current_floor,id;
    char state;
};
class system{
  public:
    lift lift1,lift2;

    void best_call(lift &lift, char priority, vector<input> &input){

      int min_up=INT_MAX,max_up=-1,min_down=INT_MAX,max_down=-1,mark,mark_min_up,mark_max_up,mark_min_down,mark_max_down,current=0,mark_current;
      for(int i=0; i<input.size(); i++){
        if(input[i].flag==0){
          if(input[i].start==lift.current_floor){
            current=1;
            mark_current=i;
            break;
          }
          else if(input[i].start>lift.current_floor&&input[i].direction=='U'&&(input[i].start-lift.current_floor)<min_up){
            min_up=input[i].start-lift.current_floor;
            mark_min_up=i;
          }
          else if(input[i].start>lift.current_floor&&input[i].direction=='D'&&(input[i].start-lift.current_floor)>max_up){
            max_up=input[i].start-lift.current_floor;
            mark_max_up=i;
          }
          else if(input[i].start<lift.current_floor&&input[i].direction=='D'&&(lift.current_floor-input[i].start)<min_down){
            min_down=lift.current_floor-input[i].start;
            mark_min_down=i;
          }
          else if(input[i].start<lift.current_floor&&input[i].direction=='U'&&(lift.current_floor-input[i].start)>max_down){
            max_down=lift.current_floor-input[i].start;
            mark_max_down=i;
          }
        }
      }

      if(current==1){
        mark=mark_current;
      }
      else if(priority=='U'){
        if(min_up!=INT_MAX){
          mark=mark_min_up;
        }
        else{
          if(max_up!=-1){
            mark=mark_max_up;
          }
          else{
            if(min_down!=INT_MAX){
              mark=mark_min_down;
            }
            else{
              mark=mark_max_down;
            }
          }
        }
      }
      else if(priority=='D'){
        if(min_down!=INT_MAX){
          mark=mark_min_down;
        }
        else{
          if(max_down!=-1){
            mark=mark_max_down;
          }
          else{
            if(min_up!=INT_MAX){
              mark=mark_min_up;
            }
            else{
              mark=mark_max_up;
            }
          }
        }
      }

      if(lift.current_floor!=input[mark].start)
        printf("---Lift%d moved from %d floor to %d floor.---\n",lift.id,lift.current_floor,input[mark].start);

      lift.state=input[mark].direction;
      lift.current_floor=input[mark].start;

      if(lift.state=='U'){
        push_route_up(input[mark].start, input[mark].end, input[mark].id, lift.route_up);
      }
      else if(lift.state=='D'){
        push_route_down(input[mark].start, input[mark].end, input[mark].id, lift.route_down);
      }

      input[mark].flag=1;

    }
    void in_way_calls(lift &lift, vector<input> &input){

      for(int i=0; i<input.size(); i++){
        if(input[i].flag==0&&input[i].direction==lift.state){
          if(lift.state=='U'){
            if(input[i].start>=lift.current_floor){
              push_route_up(input[i].start, input[i].end, input[i].id, lift.route_up);
              input[i].flag=1;
            }
          }
          if(lift.state=='D'){
            if(input[i].start<=lift.current_floor){
              push_route_down(input[i].start, input[i].end, input[i].id, lift.route_down);
              input[i].flag=1;
            }
          }
        }
      }

    }
    void route(lift &lift){

      if(lift.state=='U'){
        while(!lift.route_up.empty()){
          if(lift.current_floor!=lift.route_up.top().floor){
            printf("---Lift%d moved from %d floor to %d floor.---\n",lift.id,lift.current_floor,lift.route_up.top().floor);
            lift.current_floor=lift.route_up.top().floor;
          }
          if(lift.route_up.top().loc=='D'){
            printf("Drops %d.\n",lift.route_up.top().id);
          }
          if(lift.route_up.top().loc=='P'){
            printf("Picks %d.\n",lift.route_up.top().id);
          }
          lift.route_up.pop();
        }
      }

      if(lift.state=='D'){
        while(!lift.route_down.empty()){
          if(lift.current_floor!=lift.route_down.top().floor){
            printf("---Lift%d moved from %d floor to %d floor.---\n",lift.id,lift.current_floor,lift.route_down.top().floor);
            lift.current_floor=lift.route_down.top().floor;
          }
          if(lift.route_down.top().loc=='D'){
            printf("Drops %d.\n",lift.route_down.top().id);
          }
          if(lift.route_down.top().loc=='P'){
            printf("Picks %d.\n",lift.route_down.top().id);
          }
          lift.route_down.pop();
        }
      }

    }

  private:
    void push_route_up(int start, int end, int id, priority_queue<user, vector<user>, compare_up> &route){
      struct user u;
      u.floor=start;
      u.id=id;
      u.loc='P';
      route.push(u);
      u.floor=end;
      u.id=id;
      u.loc='D';
      route.push(u);
    }
    void push_route_down(int start, int end, int id, priority_queue<user, vector<user>, compare_down> &route){
      struct user u;
      u.floor=start;
      u.id=id;
      u.loc='P';
      route.push(u);
      u.floor=end;
      u.id=id;
      u.loc='D';
      route.push(u);
    }

};

bool complete(vector<input> input){

  int flag=1;

  for(int i=0; i<input.size(); i++){
    if(input[i].flag==0){
      flag=0;
      break;
    }
  }

  if(flag==1)
    return true;
  else
    return false;
}

int main(){

  int n,i=1;
  vector<input> input;
  struct input inp;

  cout<<"Number of entries : ";
  cin>>n;

  while(n>0){
    cin>>inp.start>>inp.direction>>inp.end;
    inp.id=i;
    inp.flag=0;
    input.push_back(inp);
    n--;
    i++;
  } //input from user

  class system elevator;

  elevator.lift1.current_floor=0;
  elevator.lift2.current_floor=0;
  elevator.lift1.id=1;
  elevator.lift2.id=2;

  while(1){
    if(!complete(input)){
      cout << "***LIFT 1***" << '\n';
      elevator.best_call(elevator.lift1,'U',input);
      elevator.in_way_calls(elevator.lift1,input);
      elevator.route(elevator.lift1);
    }
    if(!complete(input)){
      cout << "***LIFT 2***" << '\n';
      elevator.best_call(elevator.lift2,'D',input);
      elevator.in_way_calls(elevator.lift2,input);
      elevator.route(elevator.lift2);
    }
  } //run system
}
