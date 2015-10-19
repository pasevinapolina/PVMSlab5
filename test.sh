sudo make

sudo insmod ./timer.ko
sudo chmod 666 /sys/kernel/kobject_timer/timer

sudo echo "5" > /sys/kernel/kobject_timer/timer
dmesg | tail -n10
echo "Wait for 10 sec"
sleep 10
dmesg | tail -n20

sudo echo "3" > /sys/kernel/kobject_timer/timer
echo "Wait for 9 sec"
sleep 9
dmesg | tail -n12

echo "Wait for 18 sec"
sleep 18
dmesg | tail -n20

sudo rmmod timer
sudo make clean
