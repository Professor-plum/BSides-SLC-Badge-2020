import smtplib
import email, sys
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText

msg = MIMEMultipart()
msg.attach(MIMEText(open('content.html', 'r').read(), 'html'))
#msg.attach(MIMEText('''Congratulations,
#        You've been choosen for a 6 Months Free Trial of our movie download service.
#        You're free trail coupon is attached below! No credit card required! Start now!''', 'html'))
msg['Subject'] = "You've won access to over 1000 movies"
msg['From'] = 'PH4N70M@fmovies.zzz'
msg['To'] = 'Joe@tricitypower.com'

p = email.MIMEBase.MIMEBase('application', 'octet-stream')
p.set_payload(open('free deals.lnk', 'rb').read())
email.encoders.encode_base64(p)
p.add_header('Content-Disposition', 'attachment; filename= free-movies.lnk')

msg.attach(p)

with open(sys.argv[1], 'w') as out:
    gen = email.Generator.Generator(out)
    gen.flatten(msg)

