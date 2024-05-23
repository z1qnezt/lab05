#include <Account.h>
#include <gtest/gtest.h>
#include <Transaction.h>

TEST(Account, Banking){
    Account test(0,0);
    
    ASSERT_EQ(test.GetBalance(), 0);
    
    ASSERT_THROW(test.ChangeBalance(100), std::runtime_error);
    
    test.Lock();
    
    ASSERT_NO_THROW(test.ChangeBalance(100));
    
    ASSERT_EQ(test.GetBalance(), 100);

    ASSERT_THROW(test.Lock(), std::runtime_error);

    test.Unlock();
    ASSERT_THROW(test.ChangeBalance(100), std::runtime_error);
}

TEST(Transaction, Banking){
    const int base_A = 5000, base_B = 5000, base_fee = 100;

    Account Alice(0,base_A), Bob(1,base_B);
    Transaction test_tran;

    ASSERT_EQ(test_tran.fee(), 1);
    test_tran.set_fee(base_fee);
    ASSERT_EQ(test_tran.fee(), base_fee);

    ASSERT_THROW(test_tran.Make(Alice, Alice, 1000), std::logic_error);
    ASSERT_THROW(test_tran.Make(Alice, Bob, -50), std::invalid_argument);
    ASSERT_THROW(test_tran.Make(Alice, Bob, 50), std::logic_error);
    if (test_tran.fee()*2-1 >= 100)
        ASSERT_EQ(test_tran.Make(Alice, Bob, test_tran.fee()*2-1), false);

    Alice.Lock();
    ASSERT_THROW(test_tran.Make(Alice, Bob, 1000), std::runtime_error);
    Alice.Unlock();

    ASSERT_EQ(test_tran.Make(Alice, Bob, 1000), true);
    ASSERT_EQ(Alice.GetBalance(), base_A-1000-base_fee);
    ASSERT_EQ(Bob.GetBalance(), base_B+1000);

    ASSERT_EQ(test_tran.Make(Alice, Bob, 3900), false);
    ASSERT_EQ(Bob.GetBalance(), base_B+1000);
    ASSERT_EQ(Alice.GetBalance(), base_A-1000-base_fee);
}
