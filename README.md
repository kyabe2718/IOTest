# IOTest

## sample
```
$ sudo ./io_test --blksz=4096 --size=$((100 * 1024**2)) --range=$((100 * 1024**3)) --random --output=output.log /dev/sdc
$ ./plot.py output.log
```

![plot.png](./plot.png)
