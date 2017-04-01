/*
 * property.h
 * ----------
 *
 * For SFWR ENG 4F03 PA3 (Winter 2017)
 *
 * Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 * James Lee            (leejr@mcmaster.ca, MSID#001318125)
 */

#ifndef PA3_PROPERTY_H
#define PA3_PROPERTY_H

/**
 * Check if a segment with the given characters counts satisfies a property.
 *
 * @param index: The index of the property
 * @param nc0: The number of occurrences of c0 in the segment
 * @param nc1: The number of occurrences of c1 in the segment
 * @param nc2: The number of occurrences of c2 in the segment
 *
 * @return 1 if the property is satisfied; 0 otherwise
 */
int property(int index, int nc0, int nc1, int nc2);

/**
 * Check if a property can be satisfied for a segment using only critical characters.
 *
 * @param propertyIndex: The index of the property to check
 * @param segmentLength: The length of the segment
 * @param nc0: The number of character c0 already in the segment
 * @param nc1: The number of character c1 already in the segment
 * @param nc2: The number of character c2 already in the segment
 *
 * @return 1 if the property may yet be satisfied; 0 otherwise
 */
int isPossible(int propertyIndex, int segmentLength, int nc0, int nc1, int nc2);

/**
 * Check if a property can be satisfied for a segment.
 *
 * @param propertyIndex: The index of the property to check
 * @param segmentLength: The length of the segment
 * @param nc0: The number of character c0 already in the segment
 * @param nc1: The number of character c1 already in the segment
 * @param nc2: The number of character c2 already in the segment
 * @param noncrit: The number of other characters already in the segment
 *
 * @return 1 if the property may yet be satisfied; 0 otherwise
 */
int isPossibleNC(int propertyIndex, int segmentLength, int nc0, int nc1, int nc2, int noncrit);

#endif
