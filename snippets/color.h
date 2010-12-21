struct Color
{
    union
    {
        struct
        {
            uint8_t b;
            uint8_t g;
            uint8_t r;
            uint8_t a;
        };

        uint8_t[4] bytes;
        uint32_t   intval;
    };
};
