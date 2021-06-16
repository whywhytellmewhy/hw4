#include "bbcar_rpc.h"
RPCFunction rpcStop(&RPC_stop, "stop");
RPCFunction rpcCtrl(&RPC_goStraight, "goStraight");
RPCFunction rpcTurn(&RPC_turn, "turn");
//RPCFunction rpcposition(&position, "p");

extern BBCar car;

void RPC_stop (Arguments *in, Reply *out)   {
    car.stop();
    return;
}

void RPC_goStraight (Arguments *in, Reply *out)   {
    int speed = in->getArg<double>();
    car.goStraight(speed);
    return;
}

void RPC_turn (Arguments *in, Reply *out)   {
    int speed = in->getArg<double>();
    double turn = in->getArg<double>();
    car.turn(speed,turn);
    return;
}


/*void position(Arguments *in, Reply *out){
    double x = in->getArg<double>();
    double y = in->getArg<double>();
    //car.back_for_a_distance(x,y);
    car.back_parking(x,y);
    return;
}*/