/*
 * property.c
 * ----------
 *
 * For SFWR ENG 4F03 PA3 (Winter 2017)
 *
 * Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 * James Lee            (leejr@mcmaster.ca, MSID#001318125)
 */

int property(int index, int nc0, int nc1, int nc2)
{
	switch (index)
	{
		case  0: return nc0 + nc1   == nc2;
		case  1: return nc0 + nc1*2 == nc2;
		case  2: return nc0 * nc1   == nc2;
		case  3: return nc0 - nc1   == nc2;
		default: return 0;
	}
}

int isPossible(int propertyIndex, int segmentLength, int nc0, int nc1, int nc2)
{
	int tc0, tc1, tc2;

	for (tc0 = nc0; tc0 + nc1 + nc2 <= segmentLength; ++tc0)
	for (tc1 = nc1; tc0 + tc1 + nc2 <= segmentLength; ++tc1)
	{
		tc2 = segmentLength - tc0 - tc1;
		if (property(propertyIndex, tc0, tc1, tc2)) return 1;
	}

	return 0;
}

int isPossibleNC(int propertyIndex, int segmentLength, int nc0, int nc1, int nc2, int noncrit)
{
	int tc0, tc1, tc2;

	for (tc0 = nc0; tc0 + nc1 + nc2 + noncrit <= segmentLength; ++tc0)
	for (tc1 = nc1; tc0 + tc1 + nc2 + noncrit <= segmentLength; ++tc1)
	for (tc2 = nc2; tc0 + tc1 + tc2 + noncrit <= segmentLength; ++tc2)
	{
		if (property(propertyIndex, tc0, tc1, tc2)) return 1;
	}

	return 0;
}
