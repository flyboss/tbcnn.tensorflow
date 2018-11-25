

#ifndef KXSORT_H__
#define KXSORT_H__

#include <algorithm>
#include <cmath>
#include <iterator>

namespace kx {

    static const int kRadixBits = 8;
    static const size_t kInsertSortThreshold = 64;
    static const int kRadixMask = (1 << kRadixBits) - 1;
    static const int kRadixBin = 1 << kRadixBits;



    template <class T>
    struct RadixTraitsUnsigned {
        static const int nBytes = sizeof(T);
        int kth_byte (const T &x, int k) { return (x >> (kRadixBits * k)) & kRadixMask; }
        bool compare(const T &x, const T &y) { return x < y; }
    };

    template<class T>
    struct RadixTraitsSigned {
        static const int nBytes = sizeof(T);
        static const T kMSB = T(0x80) << ((sizeof(T) - 1) * 8);
        int kth_byte (const T &x, int k) {
            return ((x ^ kMSB) >> (kRadixBits * k)) & kRadixMask;
        }
        bool compare(const T &x, const T &y) { return x < y; }
    };

    template <class RandomIt, class ValueType, class RadixTraits>
    inline void insert_sort_core_(RandomIt s, RandomIt e, RadixTraits radix_traits)
    {
        for (RandomIt i = s + 1; i < e; ++i) {
            if (radix_traits.compare(*i, *(i - 1))) {
                RandomIt j;
                ValueType tmp = *i;
                *i = *(i - 1);
                for (j = i - 1; j > s && radix_traits.compare(tmp, *(j - 1)); --j) {
                    *j = *(j - 1);
                }
                *j = tmp;
            }
        }
    }

    template <class RandomIt, class ValueType, class RadixTraits, int kWhichByte>
    inline void radix_sort_core_(RandomIt s, RandomIt e, RadixTraits radix_traits)
    {
        RandomIt last_[kRadixBin + 1];
        RandomIt *last = last_ + 1;
        size_t count[kRadixBin] = {0};

        for (RandomIt i = s; i < e; ++i) {
            ++count[radix_traits.kth_byte(*i, kWhichByte)];
        }

        last_[0] = last_[1] = s;

        for (int i = 1; i < kRadixBin; ++i) {
            last[i] = last[i-1] + count[i-1];
        }

        for (int i = 0; i < kRadixBin; ++i) {
            RandomIt end = last[i-1] + count[i];
            if (end == e) { last[i] = e; break; }
            while (last[i] != end) {
                ValueType swapper = *last[i];
                int tag = radix_traits.kth_byte(swapper, kWhichByte);
                if (tag != i) {
                    do {
                        std::swap(swapper, *last[tag]++);
                    } while ((tag = radix_traits.kth_byte(swapper, kWhichByte)) != i);
                    *last[i] = swapper;
                }
                ++last[i];
            }
        }

        if (kWhichByte > 0) {
            for (int i = 0; i < kRadixBin; ++i) {
                if (count[i] > kInsertSortThreshold) {
                    radix_sort_core_<RandomIt, ValueType, RadixTraits,
                            (kWhichByte > 0 ? (kWhichByte - 1) : 0)>
                            (last[i-1], last[i], radix_traits);
                } else if (count[i] > 1) {
                    insert_sort_core_<RandomIt, ValueType, RadixTraits>(last[i-1], last[i], radix_traits);
                }
            }
        }
    }

    template <class RandomIt, class ValueType, class RadixTraits>
    inline void radix_sort_entry_(RandomIt s, RandomIt e, ValueType*,
                                  RadixTraits radix_traits, uint64_t diff)
    {
        if (e - s <= (int)kInsertSortThreshold)
            insert_sort_core_<RandomIt, ValueType, RadixTraits>(s, e, radix_traits);
        else
        {
            auto bits = static_cast<int>(std::ceil(log2(diff)));
            auto bytes = static_cast<uint32_t>(std::ceil(bits / (double) 8));

            if (bytes == 1) radix_sort_core_<RandomIt, ValueType, RadixTraits, 0>(s, e, radix_traits);
            if (bytes == 2) radix_sort_core_<RandomIt, ValueType, RadixTraits, 1>(s, e, radix_traits);
            if (bytes == 3) radix_sort_core_<RandomIt, ValueType, RadixTraits, 2>(s, e, radix_traits);
            if (bytes == 4) radix_sort_core_<RandomIt, ValueType, RadixTraits, 3>(s, e, radix_traits);
            if (bytes == 5) radix_sort_core_<RandomIt, ValueType, RadixTraits, 4>(s, e, radix_traits);
            if (bytes == 6) radix_sort_core_<RandomIt, ValueType, RadixTraits, 5>(s, e, radix_traits);
            if (bytes == 7) radix_sort_core_<RandomIt, ValueType, RadixTraits, 6>(s, e, radix_traits);
            if (bytes == 8) radix_sort_core_<RandomIt, ValueType, RadixTraits, 7>(s, e, radix_traits);
        }
    }

    template <class RandomIt, class ValueType>
    inline void radix_sort_entry_(RandomIt s, RandomIt e, ValueType *)
    {
        if (ValueType(-1) > ValueType(0)) {
            radix_sort_entry_(s, e, (ValueType*)(0), RadixTraitsUnsigned<ValueType>());
        } else {
            radix_sort_entry_(s, e, (ValueType*)(0), RadixTraitsSigned<ValueType>());
        }
    }



    template <class RandomIt, class RadixTraits>
    inline void radix_sort(RandomIt s, RandomIt e, RadixTraits radix_traits, uint64_t diff)
    {
        typename std::iterator_traits<RandomIt>::value_type *dummy(0);
        radix_sort_entry_(s, e, dummy, radix_traits, diff);
    }

    template <class RandomIt>
    inline void radix_sort(RandomIt s, RandomIt e)
    {
        typename std::iterator_traits<RandomIt>::value_type *dummy(0);
        radix_sort_entry_(s, e, dummy);
    }

}

#endif
