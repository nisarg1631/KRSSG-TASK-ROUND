/*
IMPROVEMENTS

1. Updated best_call to check if there is an input at the floor where lift is initially at.
2. Updated in_way_calls to take multiple entries from same floor.
*/

#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

struct input{
  int start,end,id,flag,drop_flag,temp_flag;
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

      int min_up=INT_MAX,max_up=-1,min_down=INT_MAX,max_down=-1,mark=-1,mark_min_up,mark_max_up,mark_min_down,mark_max_down;
      for(int i=0; i<input.size(); i++){
        if(input[i].flag==0&&input[i].temp_flag==0){
          if(input[i].start>=lift.current_floor&&input[i].direction=='U'&&(input[i].start-lift.current_floor)<min_up){
            min_up=input[i].start-lift.current_floor;
            mark_min_up=i;
          }
          else if(input[i].start>lift.current_floor&&input[i].direction=='D'&&(input[i].start-lift.current_floor)>max_up){
            max_up=input[i].start-lift.current_floor;
            mark_max_up=i;
          }
          else if(input[i].start<=lift.current_floor&&input[i].direction=='D'&&(lift.current_floor-input[i].start)<min_down){
            min_down=lift.current_floor-input[i].start;
            mark_min_down=i;
          }
          else if(input[i].start<lift.current_floor&&input[i].direction=='U'&&(lift.current_floor-input[i].start)>max_down){
            max_down=lift.current_floor-input[i].start;
            mark_max_down=i;
          }
        }
      }


      if(priority=='U'){
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

      if(input[mark].start==lift.current_floor){
        lift.state=input[mark].direction;
      }
      else if(input[mark].start>lift.current_floor){
        if(input[mark].direction=='D'){
          input[mark].temp_flag=1;
          struct user u;
          u.floor=input[mark].start;
          u.id=-1;
          u.loc='D';
          lift.route_up.push(u);
        }
        lift.state='U';
      }
      else if(input[mark].start<lift.current_floor){
        if(input[mark].direction=='U'){
          input[mark].temp_flag=1;
          struct user u;
          u.floor=input[mark].start;
          u.id=-1;
          u.loc='D';
          lift.route_down.push(u);
        }
        lift.state='D';
      }
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

    void route(lift &lift, vector<input> &input){

      if(lift.state=='U'){
        while(!lift.route_up.empty()&&lift.route_up.top().floor==lift.current_floor){
          if(lift.route_up.top().id!=-1){
            if(lift.route_up.top().loc=='D'){
              printf("Drops %d.\n",lift.route_up.top().id);
              input[lift.route_up.top().id-1].drop_flag=1;
            }
            if(lift.route_up.top().loc=='P'){
              printf("Picks %d.\n",lift.route_up.top().id);
            }
          }
          lift.route_up.pop();
        }
      }

      if(lift.state=='D'){
        while(!lift.route_down.empty()&&lift.route_down.top().floor==lift.current_floor){
          if(lift.route_down.top().id!=-1){
            if(lift.route_down.top().loc=='D'){
              printf("Drops %d.\n",lift.route_down.top().id);
              input[lift.route_down.top().id-1].drop_flag=1;
            }
            if(lift.route_down.top().loc=='P'){
              printf("Picks %d.\n",lift.route_down.top().id);
            }
          }
          lift.route_down.pop();
        }
      }
    }

    void update_state(lift &lift, vector<input> &input){
      if(lift.state=='U'){
        if(!lift.route_up.empty()){
          if(lift.route_up.top().id==-1&&lift.route_up.top().loc=='P'&&(lift.route_up.top().floor-1)==(lift.current_floor+1)){
            input[lift.route_up.top().id-1].temp_flag=0;
            lift.route_up.pop();
            lift.current_floor++;
          }
          else
            lift.current_floor++;
        }
        else{
          lift.state='R';
        }
      }
      else if(lift.state=='D'){
        if(!lift.route_down.empty()){
          if(lift.route_down.top().id==-1&&lift.route_down.top().loc=='D'&&(lift.route_down.top().floor+1)==(lift.current_floor-1)){
            input[lift.route_down.top().id-1].temp_flag=0;
            lift.route_down.pop();
            lift.current_floor--;
          }
          else
            lift.current_floor--;
        }
        else{
          lift.state='R';
        }
      }
    }

    void print_state(lift &lift){
      printf("---Lift%d : Current Floor = %d, Current State = %c.---\n",lift.id,lift.current_floor,lift.state);
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


bool complete1(vector<input> input){

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
bool complete2(vector<input> input){

  int flag=1;

  for(int i=0; i<input.size(); i++){
    if(input[i].drop_flag==0){
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

  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer;
  int buff1,buff2;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("socket failed");
      exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( 8080 );

  if (bind(server_fd, (struct sockaddr *)&address,
                               sizeof(address))<0)
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0)
  {
      perror("listen");
      exit(EXIT_FAILURE);
  }
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                     (socklen_t*)&addrlen))<0)
  {
      perror("accept");
      exit(EXIT_FAILURE);
  }

  vector<input> input;
  struct input inp;
  int i=1;

  class system elevator;

  elevator.lift1.current_floor=0;
  elevator.lift2.current_floor=0;
  elevator.lift1.id=1;
  elevator.lift2.id=2;
  elevator.lift1.state='R';
  elevator.lift2.state='R';


  while(1){

    while(1){

      valread = read( new_socket , &buff1, 4);
      valread = read( new_socket , &buffer, 1);
      valread = read( new_socket , &buff2, 4);
      if(buffer=='E'){
        break;
      }

      inp.start=buff1;
      inp.direction=buffer;
      inp.end=buff2;
      inp.id=i;
      inp.flag=0;
      inp.drop_flag=0;
      inp.temp_flag=0;
      input.push_back(inp);
      i++;
    } //input from user

    if(!complete2(input)){
      elevator.print_state(elevator.lift1);
      elevator.print_state(elevator.lift2);

      if(elevator.lift1.state=='R'&&!complete1(input)){
        elevator.best_call(elevator.lift1,'U',input);
      }
      if(elevator.lift1.state!='R'){
        elevator.in_way_calls(elevator.lift1,input);
      }

      if(elevator.lift2.state=='R'&&!complete1(input)){
        elevator.best_call(elevator.lift2,'D',input);
      }
      if(elevator.lift2.state!='R'){
        elevator.in_way_calls(elevator.lift2,input);
      }


      cout << "***LIFT 1***" << '\n';
      elevator.route(elevator.lift1,input);

      cout << "***LIFT 2***" << '\n';
      elevator.route(elevator.lift2,input);

      elevator.update_state(elevator.lift1,input);
      elevator.update_state(elevator.lift2,input);

      cout<<"#############################"<<endl;

    }
    usleep(1000000);

  } //run system
}
