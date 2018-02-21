puts "\t\t\tKeygen\nIngresa tu Nombre:"
nombre = gets.chomp
puts "\t\tNombre:\t"+nombre
d=(b=nombre.upcase+"PERUCRACKERS").length
puts "\t\tSerial:\t"+b.chars.map{|c|(Hash[(Array('A'..'Z')).zip((Array('A'..'Z')).rotate(d))]).fetch(c)}.join("")
