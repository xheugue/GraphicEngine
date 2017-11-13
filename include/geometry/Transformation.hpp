#pragma once

#include "math/Matrix.hpp"
#include "Quaternion.hpp"
#include "Direction.hpp"

#define TRANSFORMATION_DIMENSION 4

/**
 * @namespace geometry
 *
 * Espace de nommage contenant les objets géométriques nécessaire pour la réalisation du moteur
 */
namespace geometry
{
    /** @class Transformation
     *
     * Classe permettant d'effectuer des transformations
     */
    template<class T>
    class Transformation
    {
    private:
        math::Matrix<T, TRANSFORMATION_DIMENSION, TRANSFORMATION_DIMENSION> transformMat; /**< Matrice de transformation */
        Transformation() /**< Constructeur par défaut */
        {

        }
    public:

        /** \brief Construit une transformation à partir d'un quaternion
         *
         * \param q Le quaternion avec lequel construire la matrice de rotation
         */
        Transformation(Quaternion<T> &q)
        {
            T reel = q.re();
            math::Vector<T, IMAGINARY_PART_DIMENSION> img = q.im();

            transformMat[0][0] = 1 - (2 * img[1] * img[1]) - (2 * img[2] * img[2]);
            transformMat[0][1] = (2 * img[0] * img[1]) - (2 * re * img[2]);
            transformMat[0][2] = (2 * img[0] * img[2]) + (2 * re * img[1]);

            transformMat[1][0] = (2 * img[0] * img[1]) + (2 * re * img[2]);
            transformMat[1][1] = 1 - (2 * img[0] * img[0]) - (2 * img[2] * img[2]);
            transformMat[1][2] = (2 * img[1] * img[2]) - (2 * re * img[0]);

            transformMat[2][0] = (2 * img[0] * img[2]) - (2 * re * img[1]);
            transformMat[2][1] = (2 * img[1] * img[2]) + (2 * re * img[0]);
            transformMat[2][2] = 1 - (2 * img[0] * img[0]) - (2 * img[1] * img[1]);

            transformMat[3][3] = 1;
        }

        /** \brief Créer une matrice de transformation à partir d'un angle et d'une direction
         *
         * \param d L'axe de rotation
         * \param angle L'angle de rotation
         */
        Transformation(Direction &d, T angle)
        {
            double cosAngle = cos(deg2rad(angle));
            double sinAngle = sin(deg2rad(angle));

            transformMat[0][0] = cosAngle + (1 - cosAngle) * (d[0] * d[0]);
            transformMat[0][1] = (1 - cosAngle) * d[0] * d[1] + (sinAngle * d[1]);
            transformMat[0][2] = (1 - cosAngle) * d[0] * d[2] + (sinAngle * d[1]);

            transformMat[1][0] = (1 - cosAngle) * d[0] * d[1] + (sinAngle * d[2]);
            transformMat[1][1] = cosAngle + (1 - cosAngle) * (d[1] * d[1]);
            transformMat[1][2] = (1 - cosAngle) * d[1] * d[2] + (sinAngle * d[0]);

            transformMat[2][0] = (1 - cosAngle) * d[0] * d[2] + (sinAngle * d[1]);
            transformMat[2][1] = (1 - cosAngle) * d[1] * d[2] + (sinAngle * d[0]);
            transformMat[2][2] = cosAngle + (1 - cosAngle) * (d[2] * d[2]);

            transformMat[3][3] = 1;
        }

        /** \brief Concatene deux transformations
         *
         * \param t La transformation a ajouté
         * \return Le résultat de la concaténation de transformation
         */
        Transformation concat(Transformation &t)
        {
            Transformation result;
            result.transformMat[0][0] = transformMat[0][0] + t.transformMat[0][0];
            result.transformMat[0][1] = transformMat[0][1] + t.transformMat[1][1];
            result.transformMat[0][2] = transformMat[0][2] + t.transformMat[2][2];

            result.transformMat[1][0] = transformMat[1][0] + t.transformMat[0][0];
            result.transformMat[1][1] = transformMat[1][1] + t.transformMat[1][1];
            result.transformMat[1][2] = transformMat[1][2] + t.transformMat[2][2];

            result.transformMat[2][0] = transformMat[2][0] + t.transformMat[0][0];
            result.transformMat[2][1] = transformMat[2][1] + t.transformMat[1][1];
            result.transformMat[2][2] = transformMat[2][2] + t.transformMat[2][2];

            result.transformMat[3][0] = transformMat[3][0] + t.transformMat[0][0];
            result.transformMat[3][1] = transformMat[3][1] + t.transformMat[1][1];
            result.transformMat[3][2] = transformMat[3][2] + t.transformMat[2][2];
        }

        /** \brief Transforme un point
         *
         * \param p Le point à transformer
         * \return Le point résultant de la transformation
         */
        Point transform(Point<T, 3> &p)
        {
            math::Vector<T, 4> v(p[0], p[1], p[2], 1);

            v = v * transformMat;

            return Point<T, 3>(v[0], v[1], v[2]);
        }

        /** \brief Transforme une sphère
         *
         * \param s La sphere à transformer
         * \return La sphere résultant de la transformation
         */
        Sphere transform(Sphere<T> &s)
        {
            math::Point<T, SPHERE_DIMENSION> center = s.getCenter();
            center = transform(center);

            return Sphere(center, s.getRadius());
        }

        /** \brief Transforme une direction
         *
         * \param d La direction à transformer
         * \return La direction résultant de la transformation
         */
        Direction transform(Direction<T, 3> &d)
        {
            math::Vector<T, 4> v(d[0], d[1], d[2], 0);

            v = v * transformMat;

            return Direction<T, 3>(v[0], v[1], v[2]);
        }

        /** \brief Créer la matrice de transformation relative à une translation
         *
         * \param x La translation à effectuer en x
         * \param y La translation à effectuer en y
         * \param z La translation à effectuer en z
         * \return La transformation équivalente à la translation mise en paramètre
         */
        static Transformation createTranslation(T x, T y, T z)
        {
            Transformation t;

            t.transformMat[0][3] = x;
            t.transformMat[1][3] = y;
            t.transformMat[2][3] = z;

            t.transformMat[3][3] = 1;

            return t;
        }

        /** \brief  Créer la matrice de transformation equivalente à la mise à l'echelle demandée
         *
         * \param x Le facteur de mise a l'echelle sur l'axe x
         * \param y Le facteur de mise a l'echelle sur l'axe y
         * \param z Le facteur de mise a l'echelle sur l'axe z
         * \return La transformation equivalente à la mise à l'echelle passé en parametre
         */
        static Transformation createScaling(T x, T y, T z)
        {
            Transformation t;

            t.transformMat[0][0] = x;
            t.transformMat[1][1] = y;
            t.transformMat[2][2] = z;

            t.transformMat[3][3] = 1;

            return t;
        }

        friend std::ostream& operator<<(std::ostream& out, Transformation& t);
    };

    std::ostream& operator<<(std::ostream& out, Transformation& t)
    {
        out << t.transformMat[0][0] << " " << t.transformMat[0][1] << " " << t.transformMat[0][2] << " | " << t.transformMat[0][3] << std::endl;
        out << t.transformMat[1][0] << " " << t.transformMat[1][1] << " " << t.transformMat[1][2] << " | " << t.transformMat[1][3] << std::endl;
        out << t.transformMat[2][0] << " " << t.transformMat[2][1] << " " << t.transformMat[2][2] << " | " << t.transformMat[2][3] << std::endl;
        out << t.transformMat[3][0] << " " << t.transformMat[3][1] << " " << t.transformMat[3][2] << " | " << t.transformMat[3][3] << std::endl;

        return out;
    }
}