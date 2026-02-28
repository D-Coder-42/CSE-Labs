from django import forms

class StudentForm(forms.Form):
    SUBJECT_CHOICES = [
        ('Math', 'Math'),
        ('Physics', 'Physics'),
        ('Chemistry', 'Chemistry'),
        ('Biology', 'Biology'),
        ('English', 'English')
    ]
    
    name = forms.CharField(max_length=100, label="Name")
    roll_no = forms.CharField(max_length=12, label="Roll Number")
    subject = forms.ChoiceField(choices=SUBJECT_CHOICES, label="Select Subject")