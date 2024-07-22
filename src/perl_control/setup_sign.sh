%!/bin/bash

% set the sign
./set_sign.pl 'M Welcome to EPIC'
sleep 0.25
./set_sign.pl 'T 0 9:00 5:00'
sleep 0.25
./set_sign.pl 'T 1 9:00 5:00'
sleep 0.25
./set_sign.pl 'T 2 9:00 5:00'
sleep 0.25
./set_sign.pl 'T 3 9:00 5:00'
sleep 0.25
./set_sign.pl 'T 4 9:00 5:00'
sleep 0.25
./set_sign.pl 'T 5 - -'
sleep 0.25
./set_sign.pl 'T 6 - -'
sleep 0.25
./set_sign.pl 'I 3'
