float power(float x, int y)
{
    float answer = x;
    if(y == 0) return 1.0;
    if(y == 1) return x;
    for(int i = 1; i < y; i++)
    {
        answer *= x;
    }
    return answer;
}
