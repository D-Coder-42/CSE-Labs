from django import forms

class FeedbackForm(forms.Form):
    student_name = forms.CharField(label="Student Name", max_length=100)
    sex = forms.ChoiceField(label="Sex", choices=[('Male', 'Male'), ('Female', 'Female')])
    course = forms.ChoiceField(
        label="Select Course",
        choices=[
            ('ASP-XML', 'ASP-XML'),
            ('DotNET', 'DotNET'),
            ('JavaPro', 'JavaPro'),
            ('Unix,C,C++', 'Unix,C,C++')
        ]
    )
    coverage = forms.ChoiceField(
        label="Technical Coverage",
        choices=[('Excellent', 'Excellent'), ('Good', 'Good'), ('Average', 'Average'), ('Poor', 'Poor')],
        widget=forms.RadioSelect
    )
    suggestions = forms.CharField(label="Suggestions", widget=forms.Textarea, required=False)
