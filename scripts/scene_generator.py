import random
num = 300

for i in range(0, num):
    print("Material sm" + str(i) + "{")
    print(".index_of_refraction = 1.0,")
    print(".reflect_precent = 0.0,")
    print(".refract_precent = 0.0,")
    print(".ambient_color = {", random.random(), ",", random.random(), ",", random.random(), "},")
    print(".diffuse_color = {1, 1, 1},")
    print(".specular_color = {1, 1, 1},")
    print(".ambient_coeff = 0.5,")
    print(".diffuse_coeff = 0.4,")
    print(".specular_coeff = 0.1,")
    print(".specular_exponent = ", random.random() * 100)
    print("};")

dist = 30
for i in range(0, num):
    radius = random.random() / 2 + 0.05
    print("spheres.push_back({Sphere({", -dist/2 + random.random() * dist, ",", radius, ",", -dist/4 + random.random() * dist, "},", radius, ")," , "&sm" + str(i), "});")
