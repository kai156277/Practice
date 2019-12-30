#include <QLineEdit>
#include <QString>
#include <QtTest>

class UnitTestTest : public QObject
{
    Q_OBJECT

public:
    UnitTestTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void test1();
    void test2_data();
    void test2();
    void test3();
    void test4_data();
    void test4();
    void simple();
    void multiple_data();
    void multiple();
};

UnitTestTest::UnitTestTest()
{
}

void UnitTestTest::initTestCase()
{
    qDebug("called before everything else");
}

void UnitTestTest::cleanupTestCase()
{
    qDebug("called after test");
}

void UnitTestTest::test1()
{
    QVERIFY(1 == 1);
}

void UnitTestTest::test2_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");

    QTest::newRow("all lower") << "hello"
                               << "HELLO";
    QTest::newRow("mixed") << "Hello"
                           << "HELLO";
    QTest::newRow("all upper") << "HELLO"
                               << "HELLO";
}

void UnitTestTest::test2()
{
    QFETCH(QString, string);
    QFETCH(QString, result);
    QCOMPARE(string.toUpper(), result);
}

void UnitTestTest::test3()
{
    QLineEdit lineEdit;
    QTest::keyClicks(&lineEdit, "hello world");

    QCOMPARE(lineEdit.text(), QString("hello world"));
}

void UnitTestTest::test4_data()
{
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<QString>("expected");

    QTestEventList list1;
    list1.addKeyClick('a');
    QTest::newRow("char") << list1 << "a";

    QTestEventList list2;
    list2.addKeyClick('a');
    ;
    list2.addKeyClick(Qt::Key_Backspace);
    QTest::newRow("there and back again") << list2 << "";
}

void UnitTestTest::test4()
{
    QFETCH(QTestEventList, events);
    QFETCH(QString, expected);

    QLineEdit lineEdit;

    events.simulate(&lineEdit);

    QCOMPARE(lineEdit.text(), expected);
}

void UnitTestTest::simple()
{
    QString str1 = QLatin1String("This is a test string");
    QString str2 = QLatin1String("This is a test string");

    QCOMPARE(str1.localeAwareCompare(str2), 0);

    QBENCHMARK
    {
        str1.localeAwareCompare(str2);
    }
}

void UnitTestTest::multiple_data()
{
    QTest::addColumn<bool>("useLocaleCompare");
    QTest::newRow("local aware compare") << true;
    QTest::newRow("standrad compare") << false;
}

void UnitTestTest::multiple()
{
    QFETCH(bool, useLocaleCompare);
    QString str1 = QLatin1String("This is a test string");
    QString str2 = QLatin1String("This is a test string");

    int result;
    if (useLocaleCompare)
    {
        QBENCHMARK
        {
            result = str1.localeAwareCompare(str2);
        }
    }
    else
    {
        QBENCHMARK
        {
            result = (str1 == str2);
        }
    }

    Q_UNUSED(result);
}

//QTEST_APPLESS_MAIN(UnitTestTest)
QTEST_MAIN(UnitTestTest)

#include "tst_UnitTestTest.moc"
