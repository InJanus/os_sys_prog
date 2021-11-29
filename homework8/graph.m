index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
data1 = [10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000];
data2 = [9015, 9634, 9784, 9971, 10101, 10317, 10504, 10657, 10973, 9044];
data3 = [10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000];
data4 = [9984, 10027, 10051, 10053, 9928, 9943, 9928, 10136, 9957, 9993];

data = [data1;data2;data3;data4];


h = bar(data, "facecolor", "r");
title ("Philosopher Problem");
xlabel ("Philosopher Trial Type");
ylabel ("Meals eaten");

print -djpg -mono graph.jpg