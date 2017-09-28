#pragma once
#include <array>
#include <cmath>

using namespace std;

namespace math
{
  template<class T, unsigned int size>
  class Vector
  {
  private:
    const unsigned int MIN_ARGS_CROSS = 3;
  protected:
    array<T, size> vec;
  public:
    Vector() {}
    ~Vector() {}
    inline T at(const unsigned int i) const { return vec.at(i); }

    Vector<T, size> cross(const Vector<T, size> &v) const
    {
      Vector res;

      if (size < MIN_ARGS_CROSS)
        throw "The vector must have at least three arguments";

      res[0] = (vec[1] * v[2]) - (vec[2] * v[1]);
      res[1] = (vec[2] * v[0]) - (vec[0] * v[2]);
      res[2] = (vec[0] * v[1]) - (vec[1] * v[0]);

      return res;
    }

    bool is_ortho(const Vector &v) const
    {
      return ((*this) * v) == 0;
    }

    bool is_null() const
    {
      for (int i = 0; i < size; ++i)
        if (isnan((*this)[i]))
          return false;
      return true;
    }

    bool is_unit() const
    {
      return norme() == 1;
    }

    float norme() const
    {
      float somme;

      for (int i = 0; i < size; ++i)
      {
        somme += vec[i] * vec[i];
      }
      return sqrt(somme);
    }
    virtual Vector<T, size> to_unit() const
    {
      Vector<T, size> v;
      for (int i = 0; i < size; i++)
        v.vec[i] = ((1 / norme()) * (vec[i]));
    }

    T operator[](unsigned int i) const
    {
      if (i >= size)
        throw "Index out of array";

        return vec[i];
    }

    T &operator[](unsigned int i)
    {
      if (i >= size)
        throw "Index out of array";

      return vec[i];
    }

    Vector<T, size> operator+(Vector<T, size> &v) const
    {
      Vector result;

      for (int i = 0; i < size; ++i)
      {
        result[i] = this->at(i) + v[i];
      }

      return result;
    }

    Vector<T, size> &operator+=(Vector<T, size> &v)
    {
      for (int i = 0; i < size; ++i)
      {
        vec[i] = vec[i] + v[i];
      }

      return (*this);
    }

    Vector<T, size> operator-() const
    {
      Vector<T, size> res;

      for (int i = 0; i < size; ++i)
        res[i] = -vec[i];

      return res;
    }

    Vector<T, size> operator-(Vector<T, size> &v) const
    {
      Vector<T, size> res;

      for (int i = 0; i < size; ++i)
        res[i] = vec[i] - v[i];

      return res;
    }

    Vector<T, size> &operator-=(Vector<T, size> &v)
    {
      for (int i = 0; i < size; ++i)
        vec[i] = vec[i] - v[i];

      return (*this);
    }

    virtual Vector<T, size> operator*(float scalar) const
    {
      Vector<T, size> v;

      for (int i = 0; i < size; ++i)
      {
        v[i] = vec[i] * scalar;
      }

      return v;
    }

    T operator*(Vector <T, size> &v) const
    {
      T somme;

      for (int i = 0; i < size; ++i)
      {
        somme += vec[i] * v[i];
      }

      return somme;
    }

    friend Vector<T, size> operator*(float scalar, Vector <T, size> &v2)
    {
      return v2 * scalar;
    }
  };
}
