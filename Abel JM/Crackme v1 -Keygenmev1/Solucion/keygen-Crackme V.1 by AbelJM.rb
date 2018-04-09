puts "\t\t\tKeygen\nIngresa tu Nombre:"
nombre = gets.chomp
puts "\t\tNombre:\t"+nombre
puts "\t\tSerial:\t"+nombre.reverse!+"-"+(nombre.gsub(/./) {|s| s.ord.to_s})
