# SnowFlake: A Fast GUID Algorithm

Snowflake is a network service for generating unique ID numbers at high scale with some simple guarantees. The Algorithm is developed by Twitter. The original repo is on [https://github.com/twitter/snowflake](https://github.com/twitter/snowflake).

You can see some details of SnowFlake in this [Blog](https://blog.twitter.com/engineering/en_us/a/2010/announcing-snowflake.html).

### Get Started

This is a head-only impelement of the SnowFlake Algorithm. The algorithm code is in [SnowFlake.h](./SnowFlake.h) and the test code is in [test/test_SnowFlake.cc](./test/test_SnowFlake.cc). You can see the usage in the test code too.

```shell
make                # complie the test
make test    # run the test
```
