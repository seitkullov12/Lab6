#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

class Matrix {
private:
    vector<vector<double>> matrica; 
    int stroki; 
    int stolbcy; 

    // Рекурсивная функция для вычисления определителя
    double determinantRecursive(const vector<vector<double>>& a) const {
        int n = (int)a.size(); 
        if (n == 1) return a[0][0]; 
        if (n == 2) return a[0][0] * a[1][1] - a[0][1] * a[1][0]; 

        double result = 0;
        // Разложение по первой строке
        for (int col = 0; col < n; col++) {
            vector<vector<double>> submatrica(n - 1, vector<double>(n - 1));
            // Заполнение подматрицы
            for (int i = 1; i < n; i++) {
                int subcol = 0;
                for (int j = 0; j < n; j++) {
                    if (j == col) continue;
                    submatrica[i - 1][subcol++] = a[i][j];
                }
            }
            double sign = (col % 2 == 0) ? 1 : -1; 
            result += sign * a[0][col] * determinantRecursive(submatrica);
        }
        return result;
    }

public:
    Matrix() {
        stroki = 0;
        stolbcy = 0;
    }

    Matrix(int r, int c, double val = 0) {
        stroki = r;
        stolbcy = c;
        matrica.assign(stroki, vector<double>(stolbcy, val));
    }

    // Конструктор из двумерного вектора
    Matrix(const vector<vector<double>>& a) {
        matrica = a;
        stroki = (int)a.size();
        if (stroki > 0) stolbcy = (int)a[0].size();
        else stolbcy = 0;
    }

    // Геттеры
    int getStroki() { return stroki; }
    int getStolbcy() { return stolbcy; }

    double& getElement(int i, int j) { return matrica[i][j]; }
    double getElement(int i, int j) const { return matrica[i][j]; }

    // Сложение матриц
    Matrix add(const Matrix& other) const {
        if (stroki != other.stroki || stolbcy != other.stolbcy) {
            cout << "Ошибка: размеры матриц не совпадают" << endl;
            return Matrix();
        }
        Matrix result(stroki, stolbcy);
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                result.matrica[i][j] = matrica[i][j] + other.matrica[i][j];
        return result;
    }

    // Вычитание матриц
    Matrix subtract(const Matrix& other) const {
        if (stroki != other.stroki || stolbcy != other.stolbcy) {
            cout << "Ошибка: размеры матриц не совпадают" << endl;
            return Matrix();
        }
        Matrix result(stroki, stolbcy);
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                result.matrica[i][j] = matrica[i][j] - other.matrica[i][j];
        return result;
    }

    // Умножение матриц
    Matrix multiply(const Matrix& other) const {
        if (stolbcy != other.stroki) {
            cout << "Ошибка: число столбцов первой не равно числу строк второй" << endl;
            return Matrix();
        }
        Matrix result(stroki, other.stolbcy);
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < other.stolbcy; j++)
                for (int k = 0; k < stolbcy; k++)
                    result.matrica[i][j] += matrica[i][k] * other.matrica[k][j];
        return result;
    }

    // Умножение на скаляр (число)
    Matrix multiply(double scalar) const {
        Matrix result(stroki, stolbcy);
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                result.matrica[i][j] = matrica[i][j] * scalar;
        return result;
    }

    // Деление на скаляр (число)
    Matrix divide(double scalar) const {
        if (scalar == 0) {
            cout << "Ошибка: деление на ноль" << endl;
            return Matrix();
        }
        Matrix result(stroki, stolbcy);
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                result.matrica[i][j] = matrica[i][j] / scalar;
        return result;
    }

    // Деление на матрицу (умножение на обратную)
    Matrix divide(const Matrix& other) const {
        Matrix inv = other.inverse();
        if (inv.stroki == 0) return Matrix();
        return multiply(inv);
    }

    // A += B (сложение с присваиванием)
    void addAssign(const Matrix& other) {
        Matrix temp = add(other);
        matrica = temp.matrica;
        stroki = temp.stroki;
        stolbcy = temp.stolbcy;
    }

    // A -= B (вычитание с присваиванием)
    void subtractAssign(const Matrix& other) {
        Matrix temp = subtract(other);
        matrica = temp.matrica;
        stroki = temp.stroki;
        stolbcy = temp.stolbcy;
    }

    // A *= B (умножение на матрицу с присваиванием)
    void multiplyAssign(const Matrix& other) {
        Matrix temp = multiply(other);
        matrica = temp.matrica;
        stroki = temp.stroki;
        stolbcy = temp.stolbcy;
    }

    // A *= скаляр (умножение на число с присваиванием)
    void multiplyAssign(double scalar) {
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                matrica[i][j] *= scalar;
    }

    // A /= скаляр (деление на число с присваиванием)
    void divideAssign(double scalar) {
        if (scalar == 0) {
            cout << "Ошибка: деление на ноль" << endl;
            return;
        }
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                matrica[i][j] /= scalar;
    }

    // A /= B (деление на матрицу с присваиванием)
    void divideAssign(const Matrix& other) {
        Matrix temp = divide(other);
        matrica = temp.matrica;
        stroki = temp.stroki;
        stolbcy = temp.stolbcy;
    }

    // Операция сравнения на равенство
    bool isEqual(const Matrix& other) const {
        if (stroki != other.stroki || stolbcy != other.stolbcy) return false;
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                if (fabs(matrica[i][j] - other.matrica[i][j]) > 1e-9)
                    return false;
        return true;
    }

    // Операция сравнения на неравенство
    bool isNotEqual(const Matrix& other) const {
        return !isEqual(other);
    }

    // Транспонирование матрицы
    Matrix transpose() const {
        Matrix result(stolbcy, stroki);
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                result.matrica[j][i] = matrica[i][j];
        return result;
    }

    // Вычисление определителя
    double determinant() const {
        if (stroki != stolbcy) {
            cout << "Ошибка: определитель только для квадратной матрицы" << endl;
            return 0;
        }
        return determinantRecursive(matrica);
    }

    // Вычисление обратной матрицы методом Гаусса
    Matrix inverse() const {
        if (stroki != stolbcy) {
            cout << "Ошибка: обратная матрица только для квадратной матрицы" << endl;
            return Matrix();
        }

        double d = determinant();
        if (fabs(d) < 1e-9) {
            cout << "Ошибка: матрица вырождена" << endl;
            return Matrix();
        }

        int n = stroki;
        // Расширенная матрица [A | E]
        vector<vector<double>> extended(n, vector<double>(2 * n, 0));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                extended[i][j] = matrica[i][j];
            extended[i][n + i] = 1; // единичная матрица справа
        }

        // Прямой ход метода Гаусса
        for (int i = 0; i < n; i++) {
            // Поиск главного элемента
            int maxRow = i;
            for (int k = i + 1; k < n; k++)
                if (fabs(extended[k][i]) > fabs(extended[maxRow][i]))
                    maxRow = k;
            swap(extended[i], extended[maxRow]);

            double pivot = extended[i][i];
            if (fabs(pivot) < 1e-9) {
                cout << "Ошибка: не удалось вычислить обратную матрицу" << endl;
                return Matrix();
            }

            // Нормировка строки
            for (int j = i; j < 2 * n; j++)
                extended[i][j] /= pivot;

            // Обнуление остальных строк
            for (int k = 0; k < n; k++) {
                if (k != i) {
                    double factor = extended[k][i];
                    for (int j = i; j < 2 * n; j++)
                        extended[k][j] -= factor * extended[i][j];
                }
            }
        }

        // Извлечение обратной матрицы из правой части
        Matrix result(n, n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result.matrica[i][j] = extended[i][n + j];
        return result;
    }

    // Возведение матрицы в степень
    Matrix power(int exponent) const {
        if (stroki != stolbcy) {
            cout << "Ошибка: степень только для квадратной матрицы" << endl;
            return Matrix();
        }

        // Нулевая степень - единичная матрица
        if (exponent == 0) {
            Matrix result(stroki, stroki);
            for (int i = 0; i < stroki; i++)
                result.matrica[i][i] = 1;
            return result;
        }

        // Отрицательная степень - обратная матрица в положительной степени
        if (exponent < 0) {
            Matrix inv = inverse();
            if (inv.stroki == 0) return Matrix();
            return inv.power(-exponent);
        }

        // Положительная степень - последовательное умножение
        Matrix result(stroki, stolbcy);
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                result.matrica[i][j] = matrica[i][j];

        for (int p = 1; p < exponent; p++) {
            Matrix temp = result.multiply(*this);
            result = temp;
        }
        return result;
    }

    // Вычисление нормы матрицы (максимальная сумма по столбцам)
    double norm() const {
        double maxSum = 0;
        for (int j = 0; j < stolbcy; j++) {
            double sum = 0;
            for (int i = 0; i < stroki; i++)
                sum += fabs(matrica[i][j]);
            if (sum > maxSum) maxSum = sum;
        }
        return maxSum;
    }

    // Проверка: является ли матрица квадратной
    bool isSquare() const { return stroki == stolbcy; }

    // Проверка: является ли матрица диагональной
    bool isDiagonal() const {
        if (!isSquare()) return false;
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                if (i != j && fabs(matrica[i][j]) > 1e-9)
                    return false;
        return true;
    }

    // Проверка: является ли матрица нулевой
    bool isZero() const {
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                if (fabs(matrica[i][j]) > 1e-9)
                    return false;
        return true;
    }

    // Проверка: является ли матрица единичной
    bool isIdentity() const {
        if (!isSquare()) return false;
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++) {
                if (i == j && fabs(matrica[i][j] - 1) > 1e-9) return false;
                if (i != j && fabs(matrica[i][j]) > 1e-9) return false;
            }
        return true;
    }

    // Проверка: является ли матрица симметрической
    bool isSymmetric() const {
        if (!isSquare()) return false;
        for (int i = 0; i < stroki; i++)
            for (int j = i + 1; j < stolbcy; j++)
                if (fabs(matrica[i][j] - matrica[j][i]) > 1e-9)
                    return false;
        return true;
    }

    // Проверка: является ли матрица верхней треугольной
    bool isUpperTriangular() const {
        if (!isSquare()) return false;
        for (int i = 1; i < stroki; i++)
            for (int j = 0; j < i; j++)
                if (fabs(matrica[i][j]) > 1e-9)
                    return false;
        return true;
    }

    // Проверка: является ли матрица нижней треугольной
    bool isLowerTriangular() const {
        if (!isSquare()) return false;
        for (int i = 0; i < stroki; i++)
            for (int j = i + 1; j < stolbcy; j++)
                if (fabs(matrica[i][j]) > 1e-9)
                    return false;
        return true;
    }

    // Вывод матрицы на экран
    void print() const {
        for (int i = 0; i < stroki; i++) {
            for (int j = 0; j < stolbcy; j++)
                cout << setw(10) << setprecision(4) << matrica[i][j] << " ";
            cout << endl;
        }
    }

    // Ввод матрицы с клавиатуры
    void input() {
        cout << "Введите количество строк и столбцов: ";
        cin >> stroki >> stolbcy;
        matrica.assign(stroki, vector<double>(stolbcy));
        cout << "Введите элементы матрицы построчно:" << endl;
        for (int i = 0; i < stroki; i++)
            for (int j = 0; j < stolbcy; j++)
                cin >> matrica[i][j];
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    Matrix A, B; 
    int choice; 
    double scalar; 
    int exponent; 

    while (true) {
        cout << "1. Ввод матрицы A\n";
        cout << "2. Ввод матрицы B\n";
        cout << "3. Вывод матрицы A\n";
        cout << "4. Вывод матрицы B\n";
        cout << "5. Сложение A + B\n";
        cout << "6. Вычитание A - B\n";
        cout << "7. Умножение A * B\n";
        cout << "8. Умножение A * скаляр\n";
        cout << "9. Деление A / скаляр\n";
        cout << "10. Деление A / B\n";
        cout << "11. A += B\n";
        cout << "12. A -= B\n";
        cout << "13. A *= B\n";
        cout << "14. A *= скаляр\n";
        cout << "15. A /= скаляр\n";
        cout << "16. A /= B\n";
        cout << "17. Сравнение A == B\n";
        cout << "18. Сравнение A != B\n";
        cout << "19. Транспонирование A\n";
        cout << "20. Обратная матрица A\n";
        cout << "21. Возведение A в степень\n";
        cout << "22. Определитель A\n";
        cout << "23. Норма A\n";
        cout << "24. Проверка типа матрицы A\n";
        cout << "25. Проверка типа матрицы B\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "\n Ввод матрицы A \n";
            A.input();
            break;
        case 2:
            cout << "\n Ввод матрицы B \n";
            B.input();
            break;
        case 3:
            cout << "\n Матрица A \n";
            A.print();
            break;
        case 4:
            cout << "\n Матрица B \n";
            B.print();
            break;
        case 5:
            cout << "\n A + B \n";
            A.add(B).print();
            break;
        case 6:
            cout << "\n A - B \n";
            A.subtract(B).print();
            break;
        case 7:
            cout << "\n A * B \n";
            A.multiply(B).print();
            break;
        case 8:
            cout << "\nВведите скаляр: ";
            cin >> scalar;
            cout << "A * " << scalar << ":\n";
            A.multiply(scalar).print();
            break;
        case 9:
            cout << "\nВведите скаляр: ";
            cin >> scalar;
            cout << "A / " << scalar << ":\n";
            A.divide(scalar).print();
            break;
        case 10:
            cout << "\n A / B (A * B^(-1)) \n";
            A.divide(B).print();
            break;
        case 11:
            cout << "\n A += B \n";
            A.addAssign(B);
            cout << "Результат:\n";
            A.print();
            break;
        case 12:
            cout << "\n A -= B \n";
            A.subtractAssign(B);
            cout << "Результат:\n";
            A.print();
            break;
        case 13:
            cout << "\n A *= B \n";
            A.multiplyAssign(B);
            cout << "Результат:\n";
            A.print();
            break;
        case 14:
            cout << "\nВведите скаляр: ";
            cin >> scalar;
            A.multiplyAssign(scalar);
            cout << "Результат:\n";
            A.print();
            break;
        case 15:
            cout << "\nВведите скаляр: ";
            cin >> scalar;
            A.divideAssign(scalar);
            cout << "Результат:\n";
            A.print();
            break;
        case 16:
            cout << "\n A /= B \n";
            A.divideAssign(B);
            cout << "Результат:\n";
            A.print();
            break;
        case 17:
            cout << "\n Сравнение A == B \n";
            if (A.isEqual(B))
                cout << "Матрицы равны" << endl;
            else
                cout << "Матрицы не равны" << endl;
            break;
        case 18:
            cout << "\n Сравнение A != B \n";
            if (A.isNotEqual(B))
                cout << "Матрицы не равны" << endl;
            else
                cout << "Матрицы равны" << endl;
            break;
        case 19:
            cout << "\n Транспонирование A \n";
            A.transpose().print();
            break;
        case 20:
            cout << "\n Обратная матрица A \n";
            A.inverse().print();
            break;
        case 21:
            cout << "\nВведите степень: ";
            cin >> exponent;
            cout << "A^" << exponent << ":\n";
            A.power(exponent).print();
            break;
        case 22:
            cout << "\n Определитель A \n";
            cout << "det(A) = " << A.determinant() << endl;
            break;
        case 23:
            cout << "\n Норма A \n";
            cout << "||A|| = " << A.norm() << endl;
            break;
        case 24:
            cout << "\n Проверка типа матрицы A \n";
            cout << "Квадратная: " << (A.isSquare() ? "да" : "нет") << endl;
            if (A.isSquare()) {
                cout << "Диагональная: " << (A.isDiagonal() ? "да" : "нет") << endl;
                cout << "Единичная: " << (A.isIdentity() ? "да" : "нет") << endl;
                cout << "Симметрическая: " << (A.isSymmetric() ? "да" : "нет") << endl;
                cout << "Верхняя треугольная: " << (A.isUpperTriangular() ? "да" : "нет") << endl;
                cout << "Нижняя треугольная: " << (A.isLowerTriangular() ? "да" : "нет") << endl;
            }
            cout << "Нулевая: " << (A.isZero() ? "да" : "нет") << endl;
            break;
        case 25:
            cout << "\n Проверка типа матрицы B \n";
            cout << "Квадратная: " << (B.isSquare() ? "да" : "нет") << endl;
            if (B.isSquare()) {
                cout << "Диагональная: " << (B.isDiagonal() ? "да" : "нет") << endl;
                cout << "Единичная: " << (B.isIdentity() ? "да" : "нет") << endl;
                cout << "Симметрическая: " << (B.isSymmetric() ? "да" : "нет") << endl;
                cout << "Верхняя треугольная: " << (B.isUpperTriangular() ? "да" : "нет") << endl;
                cout << "Нижняя треугольная: " << (B.isLowerTriangular() ? "да" : "нет") << endl;
            }
            cout << "Нулевая: " << (B.isZero() ? "да" : "нет") << endl;
            break;
        case 0:
            cout << "\n Выход" << endl;
            return 0;
        default:
            cout << "\nНеверный выбор! Попробуйте еще раз.\n";
        }
    }
}