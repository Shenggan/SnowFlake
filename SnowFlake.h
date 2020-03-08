#include <stdint.h>
#include <sys/time.h>
#include <stdexcept>
#include <mutex>
#include "utils/lightlog.h"

class SnowFlake {
public:
    // At the beginning of the program, get an instance first, call initSnowFlake.
    static SnowFlake& getInstance() {
        static SnowFlake instance;
        return instance;
    }

    bool initSnowFlake(uint64_t datacenter_id, uint64_t machine_id) {
        if (datacenter_id > max_datacenter_num_ || datacenter_id < 0) {
            LLOG(ERRO) << "datacenterId can't be greater than max_datacenter_num_ or less than 0";
            return false;
        }
        if (machine_id > max_machine_num_ || machine_id < 0) {
            LLOG(ERRO) << "machineId can't be greater than max_machine_num_or less than 0";
            return false;
        }
        datacenter_id_ = datacenter_id << datacenter_left_;
        machine_id_ = machine_id << machine_left_;
        sequence_ = 0L;
        lastStmp_ = 0L;
        return true;
    }

    uint64_t nextId() {
        uint64_t currStmp = getNewstmp();

        std::unique_lock<std::mutex> lock(mutex_);
        if (currStmp < lastStmp_) {
            LLOG(ERRO) << "Clock moved backwards.  Refusing to generate id";
            exit(0);
        }
        if (currStmp == lastStmp_) {
            sequence_ = (sequence_ + 1) & max_sequence_num_;
            if (sequence_ == 0) {
                currStmp = getNextMill();
            }
        } else {
            sequence_ = 0;
        }
        lastStmp_ = currStmp;
        return (currStmp - start_stmp_) << timestmp_left_
                | datacenter_id_
                | machine_id_
                | sequence_;
    }

private:

    SnowFlake() {
        max_datacenter_num_ = 0x1 << datacenter_bit_;
        max_machine_num_ = 0x1 << machine_bit_;
        max_sequence_num_ = (0x1 << sequence_bit_) - 1;
        machine_left_ = sequence_bit_;
        datacenter_left_ = sequence_bit_ + machine_bit_;
        timestmp_left_ = sequence_bit_ + machine_bit_ + datacenter_bit_;
    }

    uint64_t getNextMill() {
        uint64_t mill = getNewstmp();
        while (mill <= lastStmp_) {
            mill = getNewstmp();
        }
        return mill;
    }

    uint64_t getNewstmp() {
        struct timeval tv;
        gettimeofday(&tv, NULL);

        uint64_t time = tv.tv_usec;
        time /= 1000;
        time += (tv.tv_sec * 1000);
        return time;
    }
    // init time relay, another way to solve timestamp overflow.
    uint64_t start_stmp_ = 0;
    uint64_t sequence_bit_ = 5;
    uint64_t machine_bit_ = 5;
    uint64_t datacenter_bit_ = 12;

    uint64_t max_datacenter_num_;
    uint64_t max_machine_num_;
    uint64_t max_sequence_num_;

    uint64_t machine_left_;
    uint64_t datacenter_left_;
    uint64_t timestmp_left_;

    uint64_t datacenter_id_;
    uint64_t machine_id_;
    uint64_t sequence_;
    uint64_t lastStmp_;

    std::mutex mutex_;
};
