#ifndef REALM_BENCHMARK_HPP
#define REALM_BENCHMARK_HPP

#include <realm.hpp>

#include "results.hpp"  // Results
#include "timer.hpp"    // Timer

namespace realm {
namespace test_util {

class Benchmark
{

    std::string lead_text();
    std::string ident();

    double warmup(SharedGroup&);

public:
    virtual const char* name() const = 0;
    virtual void before_all(SharedGroup&) {}
    virtual void after_all(SharedGroup&) {}
    virtual void before_each(SharedGroup&) {}
    virtual void after_each(SharedGroup&) {}
    virtual void operator()(SharedGroup&) = 0;

    virtual double min_warmup_time() {
        return 0.05;
    }
    virtual size_t max_warmup_reps() {
        return 100;
    }
    virtual double min_time() {
        return 0.1;
    }
    virtual size_t min_reps() {
        return 1000;
    }
    virtual size_t max_reps() {
        return 10000;
    }

    void run(Results& results);
};

template<DataType data_type, bool nullable = false>
class WithOneColumn : public Benchmark {
protected:
    void before_all(SharedGroup& sg)
    {
        WriteTransaction tr(sg);
        TableRef t = tr.add_table("table");
        t->add_column(data_type, "first", nullable);
        tr.commit();
    }

    void after_all(SharedGroup& sg)
    {
        // WriteTransaction doesn't have remove_table :-/
        Group& g = sg.begin_write();
        g.remove_table("table");
        sg.commit();
    }
};

template<class WithClass, size_t N>
class AddEmptyRows : public WithClass {

    void operator()(SharedGroup& sg)
    {
        WriteTransaction tr(sg);
        TableRef t = tr.get_table(0);
        t->add_empty_row(N);
        tr.commit();
    }
};

template<class WithClass, size_t N>
class WithEmptyRows : public WithClass {

    void before_all(SharedGroup& sg)
    {
        WithClass::before_all(sg);

        WriteTransaction tr(sg);
        TableRef t = tr.get_table(0);
        t->add_empty_row(N);
        tr.commit();
    }
};

template<class Benchmark>
void bench(Results& results) {
    Benchmark benchmark;
    benchmark.run(results);
}

} // namespace test_util
} // namespace realm

#endif // REALM_BENCHMARK_HPP
