import os
import random
import math


# Calculate distance btw two color
def color_similarity(base_col_val, oth_col_val):
    return math.sqrt(sum((base_col_val[i] - oth_col_val[i]) ** 2 for i in range(3)))


def get_rgb(how_many):
    colors = []
    for i in range(how_many):
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)
        colors.append((r, g, b))
    return colors


def main():
    elements = ["H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K",
                "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr",
                "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I",
                "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb",
                "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr",
                "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf",
                "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og"]
    with open("icons" + os.sep + "template_linear.svg", "r") as file:
        temp = file.readlines()
        template = "".join(temp)
    for element in elements:
        new_template = template.replace('REPLACEWITHELEMENT', f'{element}')
        while True:
            new_rgbs = get_rgb(3)
            # If color's are far enough from each other, break from while-loop
            if color_similarity(new_rgbs[0], new_rgbs[2]) > 200 and \
                    color_similarity(new_rgbs[0], new_rgbs[1]) > 200 and \
                    color_similarity(new_rgbs[1], new_rgbs[2]) > 200:
                break
        for new_rgb in new_rgbs:
            new_template = new_template.replace("rgb(255,255,255)", f"rgb{new_rgb}", 1)
        with open("icons" + os.sep + f"{element}.svg", "w") as file:
            file.write(new_template)


if __name__ == "__main__":
    main()
