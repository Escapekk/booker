#include<map>
#include<string>
#include <iostream>

#include "booker.h"



int main() {
	Booker booker;

	string input = "";
	while (getline(cin, input)) {
		if (input == "") {
			booker.print();
			break;
		}
		else {
			booker.dealInput(input);
		}
	}
	return 0;
}

/*
case 1:
abcdefghijklmnopqrst1234567890
U001 2016-06-02 22:00~22:00 A
U002 2017-08-01 19:00~22:00 A
U003 2017-08-02 13:00~17:00 B
U004 2017-08-03 15:00~16:00 C
U005 2017-08-05 09:00~11:00 D

abcdefghijklmnopqrst1234567890
> Error: the booking is invalid!
U001 2016-06-02 22:00~22:00 A
> Error: the booking is invalid!
U002 2017-08-01 19:00~22:00 A
> Success: the booking is accepted!
U003 2017-08-02 13:00~17:00 B
> Success: the booking is accepted!
U004 2017-08-03 15:00~16:00 C
> Success: the booking is accepted!
U005 2017-08-05 09:00~11:00 D
> Success: the booking is accepted!

> �������
> ---
> ����:A
> 2017-08-01 19:00~22:00 200Ԫ
> С�ƣ�200Ԫ
>
> ����:B
> 2017-08-02 13:00~17:00 200Ԫ
> С�ƣ�200Ԫ
>
> ����:C
> 2017-08-03 15:00~16:00 50Ԫ
> С�ƣ�50Ԫ
>
> ����:D
> 2017-08-05 09:00~11:00 80Ԫ
> С�ƣ�80Ԫ
> ---
> �ܼƣ�530Ԫ

case 2:
U002 2017-08-01 19:00~22:00 A
U003 2017-08-01 18:00~20:00 A
U002 2017-08-01 19:00~22:00 A C
U002 2017-08-01 19:00~22:00 A C
U003 2017-08-01 18:00~20:00 A
U003 2017-08-02 13:00~17:00 B

U002 2017-08-01 19:00~22:00 A
> Success: the booking is accepted!
U003 2017-08-01 18:00~20:00 A
> Error: the booking conflicts with existing bookings!
U002 2017-08-01 19:00~22:00 A C
> Success: the booking is accepted!
U002 2017-08-01 19:00~22:00 A C
> Error: the booking being cancelled does not exist!
U003 2017-08-01 18:00~20:00 A
> Success: the booking is accepted!
U003 2017-08-02 13:00~17:00 B
> Success: the booking is accepted!

> �������
> ---
> ����:A
> 2017-08-01 18:00~20:00 160Ԫ
> 2017-08-01 19:00~22:00 ΥԼ�� 100Ԫ
> С�ƣ�260Ԫ
>
> ����:B
> 2017-08-02 13:00~17:00 200Ԫ
> С�ƣ�200Ԫ
>
> ����:C
> С�ƣ�0Ԫ
>
> ����:D
> С�ƣ�0Ԫ
> ---
> �ܼƣ�460Ԫ
*/