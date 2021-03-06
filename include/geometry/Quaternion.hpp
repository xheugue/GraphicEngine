#pragma once

#include "math/Vector.hpp"
#include "geometry/Direction.hpp"
#include "geometry/Point.hpp"
#include "geometry/Plane.hpp"

#include <cmath>
#include <iostream>

#define PI 3.14159265358 /**< Valeur approché du nombre PI */
#define RADIAN_COEFFICIENT 180 /**< Coefficient pour transformer les degrés en radiant */
#define deg2rad(x) ((((x) * PI) / RADIAN_COEFFICIENT)) /**< Convertit les degrés en radiant */

#define QUATERNION_DIMENSION 4 /**< Nombre de membre dans le quaternion */
#define IMAGINARY_PART_DIMENSION 3 /**< Nombre de membre pour la partie imaginaire du quaternion */

/**
 * @namespace geometry
 *
 * Espace de nommage contenant les objets géométriques nécessaire pour la réalisation du moteur
 */
namespace geometry
{
    /**
     * @class Quaternion
     *
     * Objet mathematiques permettant de traiter les rotations complexes
     */
    template <class T>
    class Quaternion
    {
    private:
        math::Vector<T, QUATERNION_DIMENSION> members; /**< Les membres du quaternion  */
    public:

        /** \brief Construit un quaternion à partir de ses membres
         *
         * \param members Les membres du quaternion (reel, im1, im2, im3)
         */
        Quaternion(const math::Vector<T, QUATERNION_DIMENSION> &members) : members(members)
        {

        }

        /** \brief Définit
         *
         * \param rotation l'angle de rotation
         * \param dir La direction vers laquelle doit s'effectuer la rotation
         *
         */
        Quaternion(const float rotation, const Direction<T, 3> &dir)
        {
            float sinAngle = ROUND(std::sin(deg2rad(rotation / 2)));

            members[0] = ROUND(std::cos(deg2rad(rotation / 2)));
            members[1] = dir[0] * sinAngle;
            members[2] = dir[1] * sinAngle;
            members[3] = dir[2] * sinAngle;
        }

        /** \brief Calcul le conjugué du quaternion
         *
         * \return Le conjugué du quaternion
         *
         */
        Quaternion conjugate() const
        {
            return Quaternion(math::Vector<T, QUATERNION_DIMENSION>{members[0], -members[1], -members[2], -members[3]});
        }

        /** \brief Calcule la norme du quaternion
         *
         * \return La norme du quaternion
         */
        float norm() const
        {
            return members.norm();
        }

        /** \brief Permet d'accéder à la partie imaginaire du quaternion
         *
         * \return La partie imaginaire du quaternion
         */
        math::Vector<T, IMAGINARY_PART_DIMENSION> im() const
        {
            math::Vector<T, IMAGINARY_PART_DIMENSION> imag{members[1], members[2], members[3]};

            return imag;
        }

        /** \brief Calcul l'inverse du quaternion
         *
         * \return L'inverse du quaternion
         */
        Quaternion inverse() const
        {
            float theNorm = norm();
            return conjugate() / (theNorm * theNorm);
        }

        /** \brief Retourne la partie réelle du quaternion
         *
         * \return La partie réelle du quaternion
         */
        T re() const
        {
            return members[0];
        }

        math::Vector<T, QUATERNION_DIMENSION> getMembers() const
        {
            return members;
        }

        /** \brief Retourne une copie normalisé du quaternion
         *
         * \return Une copie du quaternion normalisé
         */
        Quaternion to_norm() const
        {
            return Quaternion(members.to_unit());
        }
        
        /** \brief Effectue une rotation sur un point 
         *
         * \param pt le point à transformer
         * \return Le point transforme
         */
        Point<T, 3> rotate(const Point<T, 3> &pt) const
        {
            Point<T, QUATERNION_DIMENSION> p(pt->at(0), pt->at(1), pt->at(2), 1);

            Point<T, QUATERNION_DIMENSION> rotatedP(p * members);

            return Point<T, 3>{rotatedP[0], rotatedP[1], rotatedP[2]};
        }
        
        /**
         * @brief Effectue une rotation sur une direction
         * @param d La direction a transformer
         * @return La direction transforme
         */
        Direction<T, 3> rotate(const Direction<T, 3> &d) const
        {
            return Direction<T, 3>{d[0] * members[0], d[1] * members[1], d[2] * members[2]};
        }
        
        /**
         * @brief Effectue une rotation sur un plan
         * @param p Le plan a transformer
         * @return Le plan transforme
         */
        Plane<T> rotate(const Plane<T> &p) const
        {
            return Plane<T>(rotate(p.GetP()), rotate(p.GetN()));
        }

        /** \brief Additione un scalaire au quaternion
         *
         * \param scalar Le scalaire a ajouter au quaternion
         * \return Le quaternion resultant de l'operation
         */
        Quaternion operator+ (const T scalar) const
        {
            return Quaternion(math::Vector<T, QUATERNION_DIMENSION>{members[0] + scalar,
                                                                    members[1],
                                                                    members[2],
                                                                    members[3]});
        }

        /** \brief Addition de deux quaternion
         *
         * \param q Le quaternion a additioner au quaternion courant
         * \return  Le quaternion résultant du quaternion courant
         */
        Quaternion operator+ (const Quaternion &q) const
        {
            return Quaternion(math::Vector<T, QUATERNION_DIMENSION>{members[0] + q.members[0],
                                                                    members[1] + q.members[1],
                                                                    members[2] + q.members[2],
                                                                    members[3] + q.members[3]});
        }

        /** \brief Soustrait un scalaire au quaternion
         *
         * \param scalar Le scalaire a asoustraire au quaternion
         * \return Le quaternion resultant de l'operation
         */
        Quaternion operator- (const T scalar) const
        {
            return Quaternion(math::Vector<T, QUATERNION_DIMENSION>{members[0] - scalar,
                                                                    members[1],
                                                                    members[2],
                                                                    members[3]});
        }

        /** \brief Soustraction de deux quaternion
         *
         * \param q Le quaternion a soustraire au quaternion courant
         * \return  Le quaternion résultant de la soustraction
         */
        Quaternion operator- (const Quaternion &q) const
        {
            return Quaternion(math::Vector<T, QUATERNION_DIMENSION>{members[0] - q.members[0],
                                                                    members[1] - q.members[1],
                                                                    members[2] - q.members[2],
                                                                    members[3] - q.members[3]});
        }
        
        /**
         * @brief Compare deux quaternions
         * @param q Le quaternions à comparer avec le quaternion courant
         * @return true si les deux quaternions sont egaux, false sinon
         */
        bool operator==(const Quaternion &q) const
        {
            return members[0] == q.getMembers()[0]
            && members[1] == q.getMembers()[1]
            && members[2] == q.getMembers()[2]
            && members[3] == q.getMembers()[3];
        }

        /** \brief Négation d'un quaternion
         *
         * \return La négation du quaternion
         */
        Quaternion operator- () const
        {
            return Quaternion(math::Vector<T, QUATERNION_DIMENSION>{-members[0], -members[1], -members[2], -members[3]});
        }

        /** \brief Multiplication d'un quaternion avec un scalaire
         *
         * \param scalar le scalaire a multiplier
         * \return Le quaternion resultant de l'operation
         */
        Quaternion operator* (const T scalar) const
        {
            return Quaternion(math::Vector<T, QUATERNION_DIMENSION>{members[0] * scalar,
                                                                    members[1] * scalar,
                                                                    members[2] * scalar,
                                                                    members[3] * scalar});
        }

        /** \brief Multiplication de deux quaternion
         *
         * \param q Le quaternion a multiplier
         * \return Le quaternion resultant de l'operation
         */
        Quaternion operator* (const Quaternion& q) const
        {
            return Quaternion(math::Vector<T, QUATERNION_DIMENSION>{members[0] * q.members[0] - members[1] * q.members[1] - members[2] * q.members[2] - members[3] * q.members[3],
                                                                    members[0] * q.members[1] + members[1] * q.members[0] + members[2] * q.members[3] + members[3] * q.members[2],
                                                                    members[0] * q.members[2] - members[1] * q.members[3] + members[2] * q.members[0] + members[3] * q.members[1],
                                                                    members[0] * q.members[3] + members[1] * q.members[2] - members[2] * q.members[1] + members[3] * q.members[0]});
        }


        /** \brief Division d'un quaternion avec un scalaire
         *
         * \param scalar le scalaire a diviser
         * \return  Le quaternion resultant de l'operation
         */
        Quaternion operator/ (const T scalar) const 
        {
            return Quaternion(math::Vector<T, QUATERNION_DIMENSION>{members[0] / scalar,
                                                                    members[1] / scalar,
                                                                    members[2] / scalar,
                                                                    members[3] / scalar});
        }

        /** \brief Additione un scalaire au quaternion
         *
         * \param scalar Le scalaire a ajouter au quaternion
         * \return Le quaternion resultant de l'operation
         */
        Quaternion& operator+= (const T scalar)
        {
            members[0] += scalar;

            return *this;
        }

        /** \brief Addition de deux quaternion
         *
         * \param q Le quaternion a additioner au quaternion courant
         * \return  Le quaternion résultant du quaternion courant
         */
        Quaternion& operator+= (const Quaternion &q)
        {
            members[0] += q.members[0];
            members[1] += q.members[1];
            members[2] += q.members[2];
            members[3] += q.members[3];

            return *this;
        }

        /** \brief Soustrait un scalaire au quaternion
         *
         * \param scalar Le scalaire a asoustraire au quaternion
         * \return Le quaternion resultant de l'operation
         */
        Quaternion& operator-= (const T scalar)
        {
            members[0] -= scalar;

            return *this;
        }

        /** \brief Soustraction de deux quaternion
         *
         * \param q Le quaternion a soustraire au quaternion courant
         * \return  Le quaternion résultant de la soustraction
         */
        Quaternion& operator-= (const Quaternion &q)
        {
            members[0] -= q.members[0];
            members[1] -= q.members[1];
            members[2] -= q.members[2];
            members[3] -= q.members[3];

            return *this;
        }

        /** \brief Multiplication d'un quaternion avec un scalaire
         *
         * \param scalar le scalaire a multiplier
         * \return Le quaternion resultant de l'operation
         */
        Quaternion& operator*= (const T scalar)
        {
            members[0] *= scalar;
            members[1] *= scalar;
            members[2] *= scalar;
            members[3] *= scalar;

            return *this;
        }

        /** \brief Multiplication de deux quaternion
         *
         * \param q Le quaternion a multiplier
         * \return Le quaternion resultant de l'operation
         */
        Quaternion& operator*= (const Quaternion& q)
        {
            math::Vector<T, QUATERNION_DIMENSION> newMembers{members[0] * q.members[0] - members[1] * q.members[1] - members[2] * q.members[2] - members[3] * q.members[3],
                                                             members[0] * q.members[1] + members[1] * q.members[0] + members[2] * q.members[3] + members[3] * q.members[2],
                                                             members[0] * q.members[2] - members[1] * q.members[3] + members[2] * q.members[0] + members[3] * q.members[1],
                                                             members[0] * q.members[3] + members[1] * q.members[2] - members[2] * q.members[1] + members[3] * q.members[0]};

            members = newMembers;
            return *this;
        }

        template <class U>
        friend Quaternion<U> operator+(const U scalar, const Quaternion<U> &q);

        template <class U>
        friend Quaternion<U> operator-(const U scalar, const Quaternion<U> &q);

        template <class U>
        friend std::ostream& operator<<(std::ostream &out, const Quaternion<U> &q);
    };

    template <class T>
    Quaternion<T> operator+(const T scalar, const Quaternion<T> &q)
    {
        return Quaternion<T>(math::Vector<T, QUATERNION_DIMENSION>{q.members[0] + scalar,
                                                                q.members[1],
                                                                q.members[2],
                                                                q.members[3]});
    }

    template<class T>
    Quaternion<T> operator-(const T scalar, const Quaternion<T> &q)
    {
        return Quaternion<T>(math::Vector<T, QUATERNION_DIMENSION>{q.members[0] - scalar,
                                                                q.members[1],
                                                                q.members[2],
                                                                q.members[3]});
    }

    template<class T>
    std::ostream& operator<< (std::ostream &out, const Quaternion<T> &q)
    {
        out << "Quaternion(" << q.members[0] << ", " << q.members[1] << ", " << q.members[2] << ", " << q.members[3] << ")";
        return out;
    }
}
