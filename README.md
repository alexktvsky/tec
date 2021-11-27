# TEC (Template Extension Compiler)

TEC is a source code preprocessing system. TEC adds syntax of C++ like templates to pure C. TEC reads C source files. If it finds one or more template declarations it produces a C source file containing the implementation of those templates.

Important! TEC doesn't replace C preprocessor. For TEC to work correctly, the C source files mustn't contain comments, and the #include directives must be replaced with the contents of the header files. In this way, TEC works with output of C preprocessor.

The sequence of steps is as follows:
> C preprocessor -> TEC -> C compiler

The source code is published under BSD 2-clause, the license is available [here][license].

## Usage

Let's have a look at examples.

### Example 1

The following code contains templates of generic function declaration and definition:

    template <typename T>
    T max(T a, T b);

    template <typename T>
    T max(T a, T b) {
        return ((a > b) ? a : b);
    }

    int main(void) {
        printf("%d\n", max<int>(11, 22));
        printf("%f\n", max<double>(11.1, 22.2));
        return 0;
    }


TEC will convert it to next source:

    int max_int(int a, int b);

    double max_double(double a, double b);

    int max_int(int a, int b) {
        return ((a > b) ? a : b);
    }

    double max_double(double a, double b) {
        return ((a > b) ? a : b);
    }

    int main(void) {
        printf("%d\n", max_int(0, 0));
        printf("%f\n", max_double(11.1, 22.2));
        return 0;
    }

### Example 2

The next code contain generic struct of list using templates:

    template <typename T>
    typedef struct node_s<T> node_t<T>;

    template <typename T>
    struct node_s {
        T data;
        node_t<T> *next;
        node_t<T> *prev;
    };

    template <typename T>
    typedef struct {
        node_t<T> *head;
        node_t<T> *tail;
    } list_t;


    int main(void) {
        list_t<int> l1;
        list_t<double> l2;
        return 0;
    }

The code after processing by the TEC will look like this:

    typedef struct node_s_int node_t_int;

    typedef struct node_s_double node_t_double;

    struct node_s_int {
        int data;
        node_t_int *next;
        node_t_int *prev;
    };

    struct node_s_double {
        double data;
        node_t_double *next;
        node_t_double *prev;
    };

    typedef struct {
        node_t_int *head;
        node_t_int *tail;
    } list_t_int;

    typedef struct {
        node_t_double *head;
        node_t_double *tail;
    } list_t_double;


    int main(void) {
        list_t_int l1;
        list_t_double l2;
        return 0;
    }

## Supported compilers

TEC can work with the following compilers:
* GCC
* Clang
* MinGW

## Supported systems
* GNU/Linux
* FreeBSD
* macOS
* Windows 7 and above


[//]: # (LINKS)
[license]: LICENSE
